/*
 * Copyright 2013 Michael M. Magruder (https://github.com/mikemag)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <algorithm>
#include <boost/range/adaptor/reversed.hpp>
#include <cassert>
#include <cmath>
#include <utility>

#include "shapes.h"
#include "shape-parts.h"
#include "svg-writer.h" // @TODO: remove when done with random svg's...

namespace MonkeyCAM {

using std::string;
using std::vector;

// Origin is at the center of the nose. Board extends right (regular
// foot, of course!)
BoardShape::BoardShape(string name,
                       MCFixed noseLength, MCFixed effectiveEdge,
                       MCFixed tailLength, MCFixed sidecutRadius,
                       MCFixed waistWidth, MCFixed taper,
                       std::unique_ptr<ShapeEndPart>& nosePart,
                       std::unique_ptr<ShapeEdgePart>& edgePart,
                       std::unique_ptr<ShapeEndPart>& tailPart,
                       MCFixed refStance, MCFixed setback,
                       std::unique_ptr<InsertPack>& nosePack,
                       std::unique_ptr<InsertPack>& tailPack,
                       MCFixed spacerWidth)
    : m_name(name)
    , m_noseLength(noseLength)
    , m_effectiveEdge(effectiveEdge)
    , m_tailLength(tailLength)
    , m_waistWidth(waistWidth)
    , m_sidecutRadius(sidecutRadius)
    , m_taper(taper)
    , m_nosePart(std::move(nosePart))
    , m_edgePart(std::move(edgePart))
    , m_tailPart(std::move(tailPart))
    , m_setback(setback)
    , m_refStance(refStance)
    , m_noseInserts(std::move(nosePack))
    , m_tailInserts(std::move(tailPack))
    , m_spacerWidth(spacerWidth)
    , m_maxCoreX(0)
{
  // Nose and tail width come from the sidecut depth...
  m_sidecutDepth = m_sidecutRadius -
    sqrt(((m_sidecutRadius * m_sidecutRadius) -
         (m_effectiveEdge * m_effectiveEdge / 4)).dbl());
  m_noseWidth = m_waistWidth + (m_sidecutDepth * 2) + (m_taper / 2);
  m_tailWidth = m_waistWidth + (m_sidecutDepth * 2) - (m_taper / 2);
  m_maxCoreX = m_noseLength + m_effectiveEdge + m_tailLength;

  setupInserts();
}

const Point BoardShape::leftGuideHole(const Machine& machine) const {
  return Point(-machine.guideHoleOffset(), 0);
}

const Point BoardShape::rightGuideHole(const Machine& machine) const {
  return Point(m_maxCoreX + machine.guideHoleOffset(), 0);
}

const void BoardShape::addDebugPath(std::function<DebugPath()> pathFunc) {
  // @todo: only if we're debugging...
  m_debugPaths.push_back(pathFunc());
}

// Build an overall board shape, which is the true finished shape of
// the board.
//
// Path is counterclockwise, start point at (0,0,0).
const Path& BoardShape::buildOverallPath() {
  if (m_overallPath.size() != 0) return m_overallPath;
  MCFixed noseEndX = 0;
  MCFixed noseTranX = m_noseLength;
  MCFixed eeCenterX = noseTranX + m_effectiveEdge / 2;
  MCFixed tailTranX = noseTranX + m_effectiveEdge;
  MCFixed tailEndX = tailTranX + m_tailLength;
  MCFixed noseHalfWidth = m_noseWidth / 2;
  MCFixed waistHalfWidth = m_waistWidth / 2;
  MCFixed tailHalfWidth = m_tailWidth / 2;
  auto np = m_nosePart->generate(Point(noseEndX, 0),
                                 Point(noseTranX, -noseHalfWidth));
  auto ep = m_edgePart->generate(Point(noseTranX, -noseHalfWidth),
                                 Point(eeCenterX, -waistHalfWidth),
                                 Point(tailTranX, -tailHalfWidth));
  auto tp = m_tailPart->generate(Point(tailEndX, 0),
                                 Point(tailTranX, -tailHalfWidth));
  m_overallPath.push_back_path(np);
  m_overallPath.push_back_path(ep);
  m_overallPath.push_back_path(tp);
  m_overallPath.push_back_path(MirroredPath(tp));
  m_overallPath.push_back_path(MirroredPath(ep));
  m_overallPath.push_back_path(MirroredPath(np));
  assert(m_overallPath.size() > 0);
  return m_overallPath;
}

template<class TIter>
void roundSpacerEnds(Path& path, TIter beginIt, TIter endIt,
                     Point startPoint, MCFixed endX,
                     MCFixed bpStartHandleYDelta,
                     MCFixed bpEndHandleXDelta) {
  auto start = std::find(beginIt, endIt, startPoint);
  assert(start != endIt);
  auto end = std::find_if(start, endIt,
                          [&] (const Point& p) {
                            if (startPoint.X > endX) {
                              return p.X <= endX;
                            } else {
                              return p.X >= endX;
                            }
                          });
  assert(end != endIt);
  auto curveEndControl = *end +
    ((*end - *(end - 1)).toVector2D().toUnitVector() * bpEndHandleXDelta.dbl());
  BezierPath bp {
    *start,
    Point(start->X, start->Y + bpStartHandleYDelta),
    curveEndControl,
    *end
  };
  Path newPath;
  newPath.insert(newPath.end(), beginIt, start);
  newPath.insert(newPath.end(), bp.begin(), bp.end());
  newPath.insert(newPath.end(), end, endIt);
  path = newPath;
}

// Generate the shape of the core for a board with sidewalls, and
// nose/tail inset to accomodate nose/tail spacers.
//
// The edge portion of the core extends out past the real edges by
// ~2mm. This gives us a bit of extra play side-to-side which allows
// the core and base to be slightly mis-aligned.
//
// The nose and tail portions are set inward by the width of the nose
// and tail spacers.
//
// The end portions are joined to the edge portion with a gentle arc.
const Path& BoardShape::buildCorePath(const Machine& machine) {
  if (m_corePath.size() != 0) return m_corePath;
  // 1. Offset the overall path inward by the spacer size.
  auto spacerClip = PathUtils::OffsetPath(m_overallPath, -m_spacerWidth);
  assert(spacerClip.size() == 1);

  // 2. Form squares for the nose and tail which cut the nose and tail
  // off at the end of the effective edge. These will become the
  // shapes of the nose and tail spacers.
  Path noseSpacerSquare;
  MCFixed noseSpacerX = -machine.spacerEndOverhang();
  MCFixed noseSpacerY = (m_noseWidth / 2) + machine.spacerSideOverhang();
  noseSpacerSquare.push_back(Point(noseSpacerX, -noseSpacerY));
  noseSpacerSquare.push_back(Point(m_noseLength, -noseSpacerY));
  noseSpacerSquare.push_back(Point(m_noseLength, noseSpacerY));
  noseSpacerSquare.push_back(Point(noseSpacerX, noseSpacerY));
  noseSpacerSquare.push_back(Point(noseSpacerX, -noseSpacerY));
  Path tailSpacerSquare;
  MCFixed tailSpacerX1 = m_noseLength + m_effectiveEdge;
  MCFixed tailSpacerX2 = tailSpacerX1 + m_tailLength +
    machine.spacerEndOverhang();
  MCFixed tailSpacerY = (m_tailWidth / 2) + machine.spacerSideOverhang();
  tailSpacerSquare.push_back(Point(tailSpacerX1, -tailSpacerY));
  tailSpacerSquare.push_back(Point(tailSpacerX2, -tailSpacerY));
  tailSpacerSquare.push_back(Point(tailSpacerX2, tailSpacerY));
  tailSpacerSquare.push_back(Point(tailSpacerX1, tailSpacerY));
  tailSpacerSquare.push_back(Point(tailSpacerX1, -tailSpacerY));

  // 3. Clip the squares from #2, removing the area defined by #1.
  auto spacers = PathUtils::ClipPathsDifference(
    vector<Path> { noseSpacerSquare, tailSpacerSquare }, spacerClip);
  assert(spacers.size() == 2);

  // 4. Round the inner corners of the of results.
  auto& noseSpacerPath = spacers[0];
  auto& tailSpacerPath = spacers[1];
  if (spacers[0][0].X > spacers[1][0].X) {
    std::swap(noseSpacerPath, tailSpacerPath);
  }
  // @TODO: make these control points a bit more configurable.
  roundSpacerEnds(noseSpacerPath,
                  noseSpacerPath.begin(), noseSpacerPath.end(),
                  Point(m_noseLength, -noseSpacerY), m_noseLength - 4,
                  4, -4);
  roundSpacerEnds(noseSpacerPath,
                  noseSpacerPath.rbegin(), noseSpacerPath.rend(),
                  Point(m_noseLength, noseSpacerY), m_noseLength - 4,
                  -4, -4);
  std::reverse(noseSpacerPath.begin(), noseSpacerPath.end());
  roundSpacerEnds(tailSpacerPath,
                  tailSpacerPath.rbegin(), tailSpacerPath.rend(),
                  Point(tailSpacerX1, -tailSpacerY), tailSpacerX1 + 4,
                  4, -4);
  std::reverse(tailSpacerPath.begin(), tailSpacerPath.end());
  roundSpacerEnds(tailSpacerPath,
                  tailSpacerPath.begin(), tailSpacerPath.end(),
                  Point(tailSpacerX1, tailSpacerY), tailSpacerX1 + 4,
                  -4, -4);

  // 4.5. The nose and tail spacer paths we have now are what we'll
  // need when generating cut programs later, so hang onto them.
  m_noseSpacerPath = noseSpacerPath;
  m_tailSpacerPath = tailSpacerPath;

  // 5. Offset the overall path outward by the sidewall overhang.
  auto overhang = PathUtils::OffsetPath(m_overallPath,
                                        machine.sidewallOverhang());
  assert(overhang.size() == 1);

  // 6. Clip the area from #5 with the results of #4, taking the inner
  // volume as the final core path.
  auto final = PathUtils::ClipPathsDifference(overhang, spacers);
  assert(final.size() == 1);
  m_corePath = final[0];
  return m_corePath;
}

void BoardShape::setupInserts() {
  auto stanceX = (m_refStance / 2);
  auto boardLength = m_noseLength + m_effectiveEdge + m_tailLength;
  auto boardCenterX = boardLength / 2;
  m_noseInserts->moveIntoPosition(Point(-stanceX + m_setback + boardCenterX,
                                        0));
  m_tailInserts->moveIntoPosition(Point(stanceX + m_setback + boardCenterX, 0));
  auto p = m_noseInserts->insertsPath();
  m_insertsPath.insert(m_insertsPath.end(), p.begin(), p.end());
  m_insertsPath.push_back(Point(m_noseInserts->maxPoint().X + 4, 0));
  m_insertsPath.push_back(Point(m_tailInserts->minPoint().X - 4, 0));
  p = m_tailInserts->insertsPath();
  m_insertsPath.insert(m_insertsPath.end(), p.begin(), p.end());
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// gcode generators

//------------------------------------------------------------------------------
// Base Cutout

const GCodeWriter BoardShape::generateBaseCutout(const Machine& machine) {
  auto tool = machine.tool(machine.baseCutoutTool());
  auto paths = PathUtils::OffsetPath(buildOverallPath(), -0.2);
  assert(paths.size() == 1);
  auto offsetPaths = PathUtils::OffsetPath(paths[0], tool.diameter / 2);
  assert(offsetPaths.size() == 1);
  auto op = offsetPaths[0];
  GCodeWriter g(m_name + "-base-cutout.nc", tool,
                GCodeWriter::TableTop, GCodeWriter::YIsPartCenter,
                machine.normalSpeed(), machine.baseRapidHeight());
  g.rapidToPoint(op[0]);
  g.spindleOn();
  g.emitPath(op, machine.baseCutThruHeight());
  g.rapidToPoint(op[0]);
  g.spindleOff();
  g.close();
  return g;
}

//------------------------------------------------------------------------------
// Core guide holes

const GCodeWriter BoardShape::generateGuideHoles(const Machine& machine) {
  // Guide holes: left hole is in negative X near the nose of the
  // board, right hole is beyond the end of the tail. Each hole is
  // 1/2" diameter, and placed 1/4" away from the overall board shape
  // to leave room for a 1/4" cutter releasing the core.
  auto rapidHeight = machine.bottomRapidHeight();
  auto holeDepth = machine.guideHoleDepth();
  auto holeDia = machine.guideHoleDiameter();
  auto tool = machine.tool(machine.guideHoleTool());
  GCodeWriter g(m_name + "-guide-holes.nc", tool,
                GCodeWriter::MaterialTop, GCodeWriter::YIsPartCenter,
                machine.normalSpeed(), rapidHeight);
  g.comment("Guide holes should be milled first so we can re-align the core if "
            "something goes wrong.");
  g.line();
  g.spindleOn();
  g.rapidToPoint(leftGuideHole(machine));
  g.emitIncrementalHole(holeDia, holeDepth, rapidHeight, 3);
  g.rapidToPoint(rightGuideHole(machine));
  g.emitIncrementalHole(holeDia, holeDepth, rapidHeight, 3);
  g.spindleOff();
  g.close();
  return g;
}

//------------------------------------------------------------------------------
// Core alignment marks

const Path BoardShape::alignmentMarksPath(const Machine& machine) {
  // Alignment marks, based off of the core nose/waist/tail control
  // points, set in a smidge.
  auto eeCenterX = (m_effectiveEdge / 2) + m_noseLength;
  auto boardLength = m_noseLength + m_effectiveEdge + m_tailLength;
  auto boardCenterX = boardLength / 2;
  auto markXOffset = machine.alignmentMarkOffset();
  auto markYOffset = machine.alignmentMarkOffset() +
    machine.edgeGrooveEdgeWidth();
  auto markDepth = machine.alignmentMarkDepth();
  auto deepMarkDepth = machine.alignmentMarkDeepDepth();
  Path marks;
  // Nose, two spaced a 10cm apart to make it easy to strike a good pencil line.
  marks.push_back(Point(m_spacerWidth + markXOffset, 0, markDepth));
  marks.push_back(Point(m_spacerWidth + markXOffset + 10, 0, markDepth));
  // Tail
  marks.push_back(Point(boardLength - m_spacerWidth - markXOffset, 0,
                        markDepth));
  marks.push_back(Point(boardLength - m_spacerWidth - markXOffset - 10, 0,
                        markDepth));
  // Overall center
  marks.push_back(Point(boardCenterX, (m_waistWidth / 2) - markYOffset,
                        markDepth));
  marks.push_back(Point(boardCenterX, 0, markDepth));
  marks.push_back(Point(boardCenterX, (-m_waistWidth / 2) + markYOffset,
                        markDepth));
  // EE center
  marks.push_back(Point(eeCenterX, (m_waistWidth / 2) - markYOffset,
                        markDepth));
  marks.push_back(Point(eeCenterX, 0, markDepth));
  marks.push_back(Point(eeCenterX, (-m_waistWidth / 2) + markYOffset,
                        markDepth));
  // Extra deep mark above the guide holes, to assist with
  // re-alignment of the machine in case of a crash.
  auto leftDeepMark = leftGuideHole(machine) + Point(0, MCFixed::fromInches(1));
  leftDeepMark.Z = deepMarkDepth;
  marks.push_back(leftDeepMark);
  auto rightDeepMark = rightGuideHole(machine) + Point(0,
                                                       MCFixed::fromInches(1));
  rightDeepMark.Z = deepMarkDepth;
  marks.push_back(rightDeepMark);
  // Sort the marks to reduce cutter movement. Forming a graph and
  // finding the shortest path would be ideal, but more trouble than
  // it's worth for this.
  std::sort(marks.begin(), marks.end());
  return marks;
}

const GCodeWriter BoardShape::generateCoreAlignmentMarks(
  const Machine& machine)
{
  auto marks = alignmentMarksPath(machine);
  auto tool = machine.tool(machine.alignmentMarkTool());
  GCodeWriter g(m_name + "-core-alignment-marks.nc", tool,
                GCodeWriter::MaterialTop, GCodeWriter::YIsPartCenter,
                machine.normalSpeed(), machine.bottomRapidHeight());
  g.spindleOn();
  for (auto& p : marks) {
    g.rapidToPoint(p);
    g.feedToPoint(p);
  }
  g.rapidToPoint(marks.back());
  g.spindleOff();
  g.close();
  return g;
}

//------------------------------------------------------------------------------
// Core edge groove

const GCodeWriter BoardShape::generateCoreEdgeGroove(const Machine& machine) {
  auto overallPath = buildOverallPath();
  auto tool = machine.tool(machine.edgeGrooveTool());
  auto edgeWidth = machine.edgeGrooveEdgeWidth(); // Entire edge, not just tines
  auto grooveWidth = machine.sidewallOverhang() + edgeWidth;
  assert(grooveWidth >= tool.diameter);

  // Debugging... true inside and outside groove edges.
  addDebugPath([&] {
      return DebugPath { "Edge Groove Inner", "rgb(255,255,0)",
          PathUtils::OffsetPath(overallPath, -edgeWidth)[0] };
    });
  addDebugPath([&] {
      return DebugPath { "Edge Groove Inner", "rgb(255,255,0)",
          PathUtils::OffsetPath(overallPath, grooveWidth - edgeWidth)[0] };
    });

  // Outter path is exact, inner path is exact. Build others inbetween
  // as appropriate.
  auto stepOffset = tool.diameter * machine.edgeGrooveOverlapPercentage();
  auto currentOffset = machine.sidewallOverhang() - (tool.diameter / 2);
  auto endOffset = -machine.edgeGrooveEdgeWidth() + (tool.diameter / 2);
  vector<vector<Path>> groovePathSets;
  while (true) {
    auto paths = PathUtils::OffsetPath(overallPath, currentOffset);
    assert(paths.size() == 1);
    groovePathSets.push_back(paths);
    addDebugPath([&] {
        return DebugPath { "tmp1", "rgb(255,0,255)", paths[0] };
      });
    if (currentOffset == endOffset) break;
    currentOffset -= stepOffset;
    if (currentOffset < endOffset) currentOffset = endOffset;
  }
  GCodeWriter g(m_name + "-core-edge-groove.nc", tool,
                GCodeWriter::MaterialTop, GCodeWriter::YIsPartCenter,
                machine.normalSpeed(), machine.bottomRapidHeight());
  g.spindleOn();
  g.emitPathSets(groovePathSets, true, machine.bottomRapidHeight(), 0,
                 machine.normalSpeed());
  g.spindleOff();
  g.close();
  return g;
}

//------------------------------------------------------------------------------
// Insert bowls and holes

// Moves from the edge of the previous ring to the edge of the new ring.
void emitInsertBowlRing(GCodeWriter& g, MCFixed prevDia, MCFixed dia,
                        MCFixed depth) {
  g.feedToPoint(Point(0, (dia - prevDia) / 2, depth));
  g.emitIncrementalCircle(dia);
}

// Assumes the cutter is at the center of the bowl, at the material top.
void emitBowl(GCodeWriter& g, MCFixed outterRimDia, MCFixed outterRimDepth) {
  vector<std::pair<MCFixed, MCFixed>> bowl {
    { outterRimDia, outterRimDepth },
    { MCFixed::fromInches(0.6928), MCFixed::fromInches(-0.0070) },
    { MCFixed::fromInches(0.6404), MCFixed::fromInches(-0.0065) },
    { MCFixed::fromInches(0.5878), MCFixed::fromInches(-0.0059) },
    { MCFixed::fromInches(0.5348), MCFixed::fromInches(-0.0054) },
    { MCFixed::fromInches(0.4818), MCFixed::fromInches(-0.0049) },
    { MCFixed::fromInches(0.4284), MCFixed::fromInches(-0.0044) },
    { MCFixed::fromInches(0.3750), MCFixed::fromInches(-0.0039) }
  };
  g.setIncremental();
  auto lastDia = g.tool().diameter; // By definition, the tool makes a hole ;)
  for (auto& p : bowl) {
    emitInsertBowlRing(g, lastDia, p.first, p.second);
    lastDia = p.first;
  }
  g.setAbsolute();
}

void emitInsert(GCodeWriter& g, const Machine& machine,
                MCFixed heightAboveMaterial) {
  auto outterRimDia = machine.insertRimDiameter();
  auto outterRimDepth = machine.insertRimDepth();
  auto insertCenterHoleDia = machine.insertHoleDiameter();
  auto insertCenterHoleDepth = -machine.coreBlankThickness();
  g.comment("A single insert");
  g.commentF("  Outter rim diameter = %s\"",
    outterRimDia.inchesStr().c_str());
  g.commentF("  Outter rim depth    = %s\"",
    outterRimDepth.inchesStr().c_str());
  g.commentF("  Shaft diameter      = %s\"",
    insertCenterHoleDia.inchesStr().c_str());
  g.commentF("  Shaft depth         = %s\"",
    insertCenterHoleDepth.inchesStr().c_str());
  auto startPosition = g.currentPosition();
  if (heightAboveMaterial > 0) {
    g.feedToPoint(g.currentPosition() + Point(0, 0, -heightAboveMaterial));
  }
  auto holeCenter = g.currentPosition();
  emitBowl(g, outterRimDia, outterRimDepth);
  g.feedToPoint(holeCenter);
  g.emitIncrementalHole(insertCenterHoleDia, insertCenterHoleDepth, 0, 3);
  // Run the bowl again to clean it up.
  emitBowl(g, outterRimDia, outterRimDepth);
  g.rapidToPoint(startPosition);
}

const GCodeWriter BoardShape::generateInsertHoles(const Machine& machine) {
  auto tool = machine.tool(machine.insertHolesTool());
  auto rapidHeight = machine.bottomRapidHeight();
  GCodeWriter g(m_name + "-core-insert-holes.nc", tool,
                GCodeWriter::MaterialTop, GCodeWriter::YIsPartCenter,
                machine.normalSpeed(), rapidHeight);
  g.spindleOn();
  for (auto& p : m_insertsPath) {
    g.rapidToPoint(p);
    emitInsert(g, machine, rapidHeight);
  }
  g.spindleOff();
  g.close();
  return g;
}

//------------------------------------------------------------------------------
// Core top profile

// @TODO: add a decent roughing pass for the nose and tail regions.

// @TODO: consider an option to make a roughing pass for the whole
// thing. I.e., cut all of it 0.020" above the final part, then speed
// thru a final finish pass.

const GCodeWriter BoardShape::generateTopProfile(const Machine& machine,
                                                 BoardProfile profile) {
  auto tool = machine.tool(machine.topProfileTool());
  // Fatten up the overall path by 1/4" to allow room for a 1/4"
  // cutter to remove the core without having to chew thru a shit-ton
  // of material.
  auto offsetPaths = PathUtils::OffsetPath(
    buildOverallPath(),
    machine.tool(machine.coreCutoutTool()).diameter);
  assert(offsetPaths.size() == 1);
  auto overallOffset = offsetPaths[0];
  // Offset the profile path to account for the width of the cutter.
  auto profileOffset = ToolOffsetPath(profile.path(), tool.diameter);
  // Offset the outter path until the offset disappears.
  auto overlap = machine.topProfileOverlapPercentage();
  vector<vector<Path>> pathSets;
  for (int i = 1; i < 100; i++) { // Artifical upper limit
    auto resultPaths = PathUtils::OffsetPath(overallOffset,
                                             -tool.diameter * overlap * i);
    if (resultPaths.empty()) break; // All done!
    // Deform each path with the profile.
    for (auto& path : resultPaths) {
      std::reverse(path.begin(), path.end());
      path = ProfiledPath(path, profileOffset);
    }
    pathSets.push_back(resultPaths);
  }
  auto rapidHeight = machine.topRapidHeight();;
  GCodeWriter g(m_name + "-top-profile.nc", tool,
                GCodeWriter::TableTop, GCodeWriter::YIsPartCenter,
                machine.topProfileDeepSpeed(), rapidHeight);
  g.spindleOn();
  g.emitPathSets(pathSets, true, rapidHeight,
                 machine.topProfileLeadinLength(),
                 machine.topProfileTransitionSpeed());
  g.spindleOff();
  g.close();
  return g;
}

//------------------------------------------------------------------------------
// Nose and tail spacer cutouts

const GCodeWriter BoardShape::generateNoseTailSpacerCutout(
  const Machine& machine)
{
  auto tool = machine.tool(machine.baseCutoutTool());
  auto nosePath = m_noseSpacerPath;
  auto tailPath = m_tailSpacerPath;

  // Shift the nose right so it starts at X=0.
  auto nShift = -nosePath[0].X;
  assert(nShift >= 0);
  std::transform(nosePath.begin(), nosePath.end(), nosePath.begin(),
                 [&](const Point& p) { return p + Point(nShift, 0); });

  // Shift the tail left so it comes within 2cm of the nose path.
  auto nmax = std::max_element(nosePath.begin(), nosePath.end(),
                               [](const Point& a, const Point& b) {
                                 return a.X < b.X;
                               });
  auto tShift = nmax->X + 2 - tailPath[0].X;
  assert(tShift <= 0);
  std::transform(tailPath.begin(), tailPath.end(), tailPath.begin(),
                 [&](const Point& p) { return p + Point(tShift, 0); });

  // Offset each
  auto ops = PathUtils::OffsetPath(nosePath, tool.diameter / 2);
  assert(ops.size() == 1);
  auto onp = ops[0];
  ops = PathUtils::OffsetPath(tailPath, tool.diameter / 2);
  assert(ops.size() == 1);
  auto otp = ops[0];

  // Trim out just the path that cuts the interface with the core
  auto lb = nosePath[0];
  auto ub = Point(lb.X + m_noseLength + machine.spacerEndOverhang() + 1, -lb.Y);
  auto trimmer = [&](const Point& p) {
    return (p.X < lb.X) || (p.X > ub.X) ||
    (p.Y < lb.Y) || (p.Y > ub.Y);
  };
  auto e = std::remove_if(onp.begin(), onp.end(), trimmer);
  onp.resize(std::distance(onp.begin(), e));

  lb = tailPath[0] - Point(1, 0);
  ub = Point(lb.X + m_tailLength + machine.spacerEndOverhang(), -lb.Y);
  e = std::remove_if(otp.begin(), otp.end(), trimmer);
  otp.resize(std::distance(otp.begin(), e));
  otp.erase(otp.begin()); // First element of the tail is redundant with the end

  GCodeWriter g(m_name + "-nose-tail-spacers.nc", tool,
                GCodeWriter::TableTop, GCodeWriter::YIsPartCenter,
                machine.normalSpeed(), machine.baseRapidHeight());
  auto materialLength = m_noseLength + m_tailLength + 2 +
    machine.spacerEndOverhang() * 2;
  g.headerComment();
  g.headerCommentF("Spacer material length: %s\" [%scm]",
                   materialLength.inchesStr().c_str(),
                   materialLength.str().c_str());
  g.headerComment();
  g.rapidToPoint(onp[0]);
  g.spindleOn();
  g.emitPath(onp, machine.baseCutThruHeight());
  g.rapidToPoint(otp[0]);
  g.emitPath(otp, machine.baseCutThruHeight());
  g.rapidToPoint(onp[0]);
  g.spindleOff();
  g.close();
  return g;
}

//------------------------------------------------------------------------------
// Core edge trench

// Counter-clockwise from lower left.
Path formRectanglePath(Point ll, Point ur) {
  Path p;
  p.push_back(ll);
  p.push_back(Point(ur.X, ll.Y));
  p.push_back(ur);
  p.push_back(Point(ll.X, ur.Y));
  p.push_back(ll);
  return p;
}

// Extend a path which is open, i.e., a line, on both ends the given
// length.
void extendLine(Path& p, MCFixed length) {
  assert(p.size() > 1);
  auto newStart = p[0] + ((p[0] - p[1]).toVector2D().toUnitVector() *
                          length.dbl());
  auto e = p.size() - 1;
  auto newEnd = p[e] + ((p[e] - p[e - 1]).toVector2D().toUnitVector() *
                        length.dbl());
  p[0] = newStart;
  p[e] = newEnd;
}

const GCodeWriter BoardShape::generateEdgeTrench(const Machine& machine) {
  // The outter edge of the edge trench is at the sidewall overhang.
  // We need to move the edge of the overall shape to the center of
  // the edge trench so we can offset it evenly on each side.
  auto& overallPath = buildOverallPath();
  auto etCenterAdjust = machine.sidewallOverhang() -
    (machine.edgeTrenchWidth() / 2);
  auto etCenterOverallPaths = PathUtils::OffsetPath(overallPath,
                                                    etCenterAdjust);
  assert(etCenterOverallPaths.size() == 1);
  auto etCenterOverallPath = etCenterOverallPaths[0];

  // Use two rectangles which will contain the nose and tail to trim
  // out the edge paths. These extend an extra 1 unit past the ends
  // and sides to ensure containtment of the shape.
  auto noseTrimPath = formRectanglePath(
    Point(-1, -(m_noseWidth / 2) - 1),
    Point(m_noseLength, (m_noseWidth / 2) + 1));
  auto tailTrimPath = formRectanglePath(
    Point(m_noseLength + m_effectiveEdge, -(m_tailWidth / 2) - 1),
    Point(m_noseLength + m_effectiveEdge + m_tailLength + 1,
          (m_tailWidth / 2) + 1));
  auto etCenterPaths = PathUtils::ClipPathsDifference(
    vector<Path> { etCenterOverallPath },
    vector<Path> { noseTrimPath, tailTrimPath });
  assert(etCenterPaths.size() == 1);
  auto etCenterPath = etCenterPaths[0];

  // Now pull out the individual center lines for the trenches from
  // the trimmed path. These are not real Paths since they are open.
  Path etLowerCenter;
  etLowerCenter.resize(etCenterPath.size());
  auto end = std::copy_if(
    etCenterPath.begin(), etCenterPath.end() - 1,
    etLowerCenter.begin(),
    [](const Point& p) { return p.Y < 0; });
  etLowerCenter.resize(std::distance(etLowerCenter.begin(), end));
  end = std::remove_if(
    etCenterPath.begin(), etCenterPath.end() - 1,
    [](const Point& p) { return p.Y < 0; });
  etCenterPath.resize(std::distance(etCenterPath.begin(), end));
  auto etUpperCenter = etCenterPath;

  // Extend each center line to clear the core. NB: add twice the
  // cutter diameter to leave room for the rounded corners on both
  // ends.
  //
  // @TODO: older versions of MonkeyCAM used to compute the
  // intersection with the core path, then extend from there, so older
  // extension values were quite a bit shorter. This is preferable,
  // and I should adjust this to do the same one day.
  auto tool = machine.tool(machine.coreCutoutTool());
  extendLine(etUpperCenter, machine.edgeTrenchExtension() +
             tool.diameter * 2);
  extendLine(etLowerCenter, machine.edgeTrenchExtension() +
             tool.diameter * 2);

  // Form the trench paths by offseting the two centerlines.
  auto trenches =
    PathUtils::OffsetLines(vector<Path> { etLowerCenter, etUpperCenter},
                           machine.edgeTrenchWidth() / 2);
  assert(trenches.size() == 2);

  // Offset the trenches inwards for machining.
  auto ps = PathUtils::OffsetPath(trenches[0], -tool.diameter / 2);
  assert(ps.size() == 1);
  auto t1 = ps[0];
  ps = PathUtils::OffsetPath(trenches[1], -tool.diameter / 2);
  assert(ps.size() == 1);
  auto t2 = ps[0];

  GCodeWriter g(m_name + "-edge-trench.nc", tool,
                GCodeWriter::TableTop, GCodeWriter::YIsPartCenter,
                machine.normalSpeed(), machine.topRapidHeight());
  g.rapidToPoint(t1[0]);
  g.spindleOn();
  g.emitSpiralPath(t1, machine.coreBlankThickness(), 3);
  g.rapidToPoint(t1[0]);
  g.rapidToPoint(t2[0]);
  g.emitSpiralPath(t2, machine.coreBlankThickness(), 3);
  g.rapidToPoint(t2[0]);
  g.spindleOff();
  g.close();
  return g;
}

//------------------------------------------------------------------------------
// Core top cutout
//
// The core cutout leaves "tabs" on the last pass to hold the core
// within the core blank for removal later. This makes it a lot easier
// to hold the core down if you don't have vacuum hold down.

const GCodeWriter BoardShape::generateTopCutout(const Machine& machine) {
  // Offset the core path as usual
  auto tool = machine.tool(machine.coreCutoutTool());
  auto paths = PathUtils::OffsetPath(buildCorePath(machine),
                                     tool.diameter / 2);
  assert(paths.size() == 1);
  auto op = paths[0];
  // Form a tab profile path
  Path tabProfile;
  auto quarterEELength = m_effectiveEdge / 4;
  auto boardLength = m_noseLength + m_effectiveEdge + m_tailLength;
  auto boardCenterX = boardLength / 2;
  auto leftTabX = boardCenterX - quarterEELength;
  auto rightTabX = boardCenterX + quarterEELength;
  MCFixed cutThroughDepth = MCFixed::fromInches(-0.010);
  MCFixed tabHeight = MCFixed::fromInches(0.080);
  tabProfile.push_back(Point(-1, cutThroughDepth));
  tabProfile.push_back(Point(leftTabX - 3, cutThroughDepth));
  tabProfile.push_back(Point(leftTabX - 1, tabHeight));
  tabProfile.push_back(Point(leftTabX + 1, tabHeight));
  tabProfile.push_back(Point(leftTabX + 3, cutThroughDepth));
  tabProfile.push_back(Point(rightTabX - 3, cutThroughDepth));
  tabProfile.push_back(Point(rightTabX - 1, tabHeight));
  tabProfile.push_back(Point(rightTabX + 1, tabHeight));
  tabProfile.push_back(Point(rightTabX + 3, cutThroughDepth));
  tabProfile.push_back(Point(boardLength + 1, cutThroughDepth));
  // Now deform the cutout path with the tab profile.
  auto profPath = ProfiledPath(op, tabProfile);
  GCodeWriter g(m_name + "-top-cutout.nc", tool,
                GCodeWriter::TableTop, GCodeWriter::YIsPartCenter,
                machine.normalSpeed(), machine.topRapidHeight());
  g.rapidToPoint(profPath[0]);
  g.spindleOn();
  g.emitSpiralPath(profPath, machine.coreBlankThickness(), 3);
  g.rapidToPoint(profPath[0]);
  g.spindleOff();
  g.close();
  return g;
}

} // namespace MonkeyCAM
