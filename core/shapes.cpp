/*
 * Copyright 2013-2014 Michael M. Magruder (https://github.com/mikemag)
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
#include <stack>
#include <utility>

#include "shapes.h"
#include "shape-parts.h"

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
                       MCFixed spacerWidth, MCFixed noseEdgeExt,
                       MCFixed tailEdgeExt)
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
    , m_noseEdgeExt(noseEdgeExt)
    , m_tailEdgeExt(tailEdgeExt)
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

BoardShape::~BoardShape() {
  for (auto dps : m_debugPathSets) {
    delete dps;
  }
}

const Point BoardShape::leftGuideHole(const Machine& machine) const {
  return Point(-machine.guideHoleOffset(), 0);
}

const Point BoardShape::rightGuideHole(const Machine& machine) const {
  return Point(m_maxCoreX + machine.guideHoleOffset(), 0);
}

DebugPathSet& BoardShape::addDebugPathSet(std::string header) {
  auto dps = new DebugPathSet { header };
  m_debugPathSets.push_back(dps);
  return *dps;
}

// Build an overall board shape, which is the true finished shape of
// the board.
//
// Path is counterclockwise, start point at (0,0,0).
const Path& BoardShape::buildOverallPath(const Machine& machine) {
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
  DebugPathSet& dps = addDebugPathSet("Overall shape");
  dps.addDescription(
    "<p>The overall shape of the board is the final, finished outline of the "
    "ski or snowboard. None of the generated G-Code programs actually cuts "
    "this shape, though the base shape with the edges amounts to the same "
    "thing. It is shown with inserts and other guide lines to present a fairly "
    "complete picture of the final board.</p>");
  dps.addDescription(
    "<ul>"
    "<li>Total length: %scm</li>"
    "<li>Nose / effective edge / tail length: %scm / %scm / %scm</li>"
    "<li>Nose / waist / tail width: %scm / %scm / %scm</li>"
    "<li>Taper: %scm</li>"
    "<li>Sidecut radius / depth: %scm / %scm</li>"
    "<li>Reference stance width: %scm</li>"
    "<li>Setback: %scm</li>"
    "<li>Board area: %.3fcm<sup>2</sup></li>"
    "<li>Extension of metal edge towards nose (999 = full wrap): %scm</li>"
    "<li>Extension of metal edge towards tail (999 = full wrap): %scm</li>"
    "</ul>",
    overallLength().str().c_str(),
    m_noseLength.str().c_str(), m_effectiveEdge.str().c_str(),
      m_tailLength.str().c_str(),
    m_noseWidth.str().c_str(), m_waistWidth.str().c_str(),
      m_tailWidth.str().c_str(),
    m_taper.str().c_str(),
    m_sidecutRadius.str().c_str(), m_sidecutDepth.str().c_str(),
    m_refStance.str().c_str(),
    m_setback.str().c_str(),
    PathUtils::Area(m_overallPath),
    m_noseEdgeExt.str().c_str(),
    m_tailEdgeExt.str().c_str()
  );
  dps.addPath([&] {
      return DebugPath {
        m_overallPath,
        DebugAnnotationDesc {
          "Overall shape"
        }
      };
    });
  dps.addAnnotation([&] {
      auto a = DebugAnnotation {
        DebugAnnotationDesc {
          "Width guides",
          "Guidelines to mark key widths of the board: nose/tail maximum "
          "width, waist width, and the true center of the board (shorter "
          "line).", "blue", true
        }
      };
      MCFixed trueCenterX = (m_noseLength + m_effectiveEdge + m_tailLength) / 2;
      a.addSvgFormat(
        R"(<path d="M%f %f L%f %f M%f %f L%f %f M%f %f L%f %f M%f %f L%f %f"/>)",
        noseTranX.dbl(), (-noseHalfWidth + 1).dbl(),
        noseTranX.dbl(), (noseHalfWidth - 1).dbl(),
        eeCenterX.dbl(), (-waistHalfWidth + 1).dbl(),
        eeCenterX.dbl(), (waistHalfWidth - 1).dbl(),
        trueCenterX.dbl(), (-waistHalfWidth + 5).dbl(),
        trueCenterX.dbl(), (waistHalfWidth - 5).dbl(),
        tailTranX.dbl(), (-tailHalfWidth + 1).dbl(),
        tailTranX.dbl(), (tailHalfWidth - 1).dbl()
      );
      return a;
    });
  dps.addAnnotation([&] {
      auto a = DebugAnnotation {
        DebugAnnotationDesc {
          "Inserts",
          "All inserts. The larger circle is the outter rim, the smaller "
          "circle is the shaft hole.", "blue"
        }
      };
      for (auto& p : m_insertsPath) {
        a.addSvgCircle(p, machine.insertRimDiameter());
        a.addSvgCircle(p, machine.insertHoleDiameter());
      }
      return a;
    });
  if (m_refStance > 0.0) {
    dps.addAnnotation([&] {
      auto a = DebugAnnotation {
        DebugAnnotationDesc {
          "Reference stance and setback",
          "The center insert group in each pack. By default, these groups are "
          "centered across the waist of the board, i.e., the center of the "
          "effective edge, plus the setback.",
          "green", true
        }
      };
      a.addSvgCircle(Point(eeCenterX - (m_refStance / 2) + m_setback, 0), 0.5);
      a.addSvgCircle(Point(eeCenterX + (m_refStance / 2) + m_setback, 0), 0.5);
      a.addSvgFormat(
        R"(<path d="M%f %f L%f %f"/>)",
        (eeCenterX + m_setback).dbl(), -4.0,
        (eeCenterX + m_setback).dbl(), 4.0);
      return a;
    });
  }
  return m_overallPath;
}

const void BoardShape::addCoreCenterComment(GCodeWriter& g) {
  auto centerX = m_noseLength + (m_effectiveEdge / 2);
  g.headerComment("* Center of the board in G54:");
  g.headerCommentF("    X=%s Y=0.0000 Z=0.0000", centerX.inchesStr().c_str());
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
  auto spacerClip = PathUtils::OffsetPath(buildOverallPath(machine),
                                          -m_spacerWidth);
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
  if (m_noseLength > m_spacerWidth + 4) {
    roundSpacerEnds(noseSpacerPath,
                    noseSpacerPath.begin(), noseSpacerPath.end(),
                    Point(m_noseLength, -noseSpacerY), m_noseLength - 4,
                    4, -4);
    roundSpacerEnds(noseSpacerPath,
                    noseSpacerPath.rbegin(), noseSpacerPath.rend(),
                    Point(m_noseLength, noseSpacerY), m_noseLength - 4,
                    -4, -4);
    std::reverse(noseSpacerPath.begin(), noseSpacerPath.end());
  }
  if (m_tailLength > m_spacerWidth + 4) {
    roundSpacerEnds(tailSpacerPath,
                    tailSpacerPath.rbegin(), tailSpacerPath.rend(),
                    Point(tailSpacerX1, -tailSpacerY), tailSpacerX1 + 4,
                    4, -4);
    std::reverse(tailSpacerPath.begin(), tailSpacerPath.end());
    roundSpacerEnds(tailSpacerPath,
                    tailSpacerPath.begin(), tailSpacerPath.end(),
                    Point(tailSpacerX1, tailSpacerY), tailSpacerX1 + 4,
                    -4, -4);
  }

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
  DebugPathSet& dps = addDebugPathSet("Core shape");
  dps.addDescription(
    "<p>This is the final shape of the core with sidewalls and extra room "
    "(%scm) for nose and tail spacer material. The sidewalls overhang the "
    "true edge of the board by %scm.</p>",
    m_spacerWidth.str().c_str(),
    machine.sidewallOverhang().str().c_str()
  );
  dps.addPath([&] {
      return DebugPath {
        m_corePath,
        DebugAnnotationDesc {
          "Core shape",
          "The final shape of the core, including sidewall overhang past the "
          "edges."
        }
      };
    });
  dps.addPath([&] {
      return DebugPath {
        m_overallPath,
        DebugAnnotationDesc {
          "Overall shape",
            "The final shape of the board, including edges.",
            "green", true
        }
      };
    });
  dps.addAnnotation([&] {
      auto a = DebugAnnotation {
        DebugAnnotationDesc {
          "Width guides",
          "Guidelines to mark key widths of the board: nose/tail maximum "
          "width, waist width, and the true center of the board (shorter "
          "line).", "blue", true
        }
      };
      MCFixed trueCenterX = (m_noseLength + m_effectiveEdge + m_tailLength) / 2;
      MCFixed noseTranX = m_noseLength;
      MCFixed eeCenterX = noseTranX + m_effectiveEdge / 2;
      MCFixed tailTranX = noseTranX + m_effectiveEdge;
      MCFixed noseHalfWidth = m_noseWidth / 2;
      MCFixed waistHalfWidth = m_waistWidth / 2;
      MCFixed tailHalfWidth = m_tailWidth / 2;
      a.addSvgFormat(
        R"(<path d="M%f %f L%f %f M%f %f L%f %f M%f %f L%f %f M%f %f L%f %f"/>)",
        noseTranX.dbl(), (-noseHalfWidth + 4).dbl(),
        noseTranX.dbl(), (noseHalfWidth - 4).dbl(),
        eeCenterX.dbl(), (-waistHalfWidth + 1).dbl(),
        eeCenterX.dbl(), (waistHalfWidth - 1).dbl(),
        trueCenterX.dbl(), (-waistHalfWidth + 5).dbl(),
        trueCenterX.dbl(), (waistHalfWidth - 5).dbl(),
        tailTranX.dbl(), (-tailHalfWidth + 4).dbl(),
        tailTranX.dbl(), (tailHalfWidth - 4).dbl()
      );
      return a;
    });
  return m_corePath;
}

void BoardShape::setupInserts() {
  auto stanceX = (m_refStance / 2);
  MCFixed eeCenterX = m_noseLength + m_effectiveEdge / 2;
  if (m_noseInserts) {
    m_noseInserts->moveIntoPosition(Point(-stanceX + m_setback + eeCenterX, 0));
    auto p = m_noseInserts->insertsPath();
    m_insertsPath.insert(m_insertsPath.end(), p.begin(), p.end());
    m_insertsPath.push_back(Point(m_noseInserts->maxPoint().X + 4, 0)); // Pin
  }
  if (m_tailInserts) {
    m_tailInserts->moveIntoPosition(Point(stanceX + m_setback + eeCenterX, 0));
    m_insertsPath.push_back(Point(m_tailInserts->minPoint().X - 4, 0)); // Pin
    auto p = m_tailInserts->insertsPath();
    m_insertsPath.insert(m_insertsPath.end(), p.begin(), p.end());
  }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// gcode generators

//------------------------------------------------------------------------------
// Base Cutout

const GCodeWriter BoardShape::generateBaseCutout(const Machine& machine) {
  auto tool = machine.tool(machine.baseCutoutTool());

  // 1. Get path assuming full wrap of metal edge
  auto fullWrapPath = PathUtils::OffsetPath(buildOverallPath(machine), -0.2);
  assert(fullWrapPath.size() == 1);

  // 2. Form box outlining limit of metal edges (whole board if no partial edges)
  MCFixed noseEdgeX = 0;
  if (m_noseEdgeExt != 999.0) {
    noseEdgeX += (m_noseLength - m_noseEdgeExt);
  }
  MCFixed tailEdgeX = m_noseLength + m_effectiveEdge + m_tailLength;
  if (m_tailEdgeExt != 999.0) {
    tailEdgeX -= (m_tailLength - m_tailEdgeExt);
  }
  MCFixed boxHalfWidth = std::max(std::max(m_noseWidth, m_waistWidth), m_tailWidth)/2 + 1;
  Path edgeLimitBox;
  edgeLimitBox.push_back(Point(noseEdgeX, - boxHalfWidth));
  edgeLimitBox.push_back(Point(tailEdgeX, - boxHalfWidth));
  edgeLimitBox.push_back(Point(tailEdgeX, boxHalfWidth));
  edgeLimitBox.push_back(Point(noseEdgeX, boxHalfWidth));
  edgeLimitBox.push_back(Point(noseEdgeX, -boxHalfWidth));

  // 3. Clip square
  auto clippedEdgeBox = PathUtils::ClipPathsDifference(
    vector<Path> {edgeLimitBox} , fullWrapPath);

  // 4. Use clipped square to clip base cutout shape
  auto paths = PathUtils::ClipPathsDifference(
    vector<Path> {m_overallPath} , clippedEdgeBox);
  assert(paths.size() == 1);
  auto trueBasePath = paths[0];

  // 5. Offset to get tool path
  auto offsetPaths = PathUtils::OffsetPath(trueBasePath, tool.diameter / 2);
  assert(offsetPaths.size() == 1);
  auto op = offsetPaths[0];

  DebugPathSet& dps = addDebugPathSet("Base Cutout");
  dps.addPath([&] {
      return DebugPath {
        op,
        DebugAnnotationDesc {
          "Base cutout path",
          "The path used to cut the base."
        }
      };
    });
  dps.addPath([&] {
      return DebugPath {
        trueBasePath,
        DebugAnnotationDesc {
          "Base",
            "The final shape of the base material.",
            "orange", true
        }
      };
    });
  dps.addPath([&] {
      return DebugPath {
        m_overallPath,
        DebugAnnotationDesc {
          "Overall shape",
            "The final shape of the board, including edges.",
            "green", true
        }
      };
    });
  GCodeWriter g(m_name + "-base-cutout.nc", tool,
                GCodeWriter::TableTop, GCodeWriter::YIsPartCenter,
                machine.normalSpeed(), machine.baseRapidHeight());
  addCoreCenterComment(g);
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
  DebugPathSet& dps = addDebugPathSet("Guide Holes");
  GCodeWriter g(m_name + "-guide-holes.nc", tool,
                GCodeWriter::MaterialTop, GCodeWriter::YIsPartCenter,
                machine.normalSpeed(), rapidHeight);
  g.comment("Guide holes should be milled first so we can re-align the core if "
            "something goes wrong.");
  addCoreCenterComment(g);
  g.line();
  g.spindleOn();
  g.rapidToPoint(leftGuideHole(machine));
  g.emitIncrementalHole(holeDia, holeDepth, rapidHeight, 3);
  g.rapidToPoint(rightGuideHole(machine));
  g.emitIncrementalHole(holeDia, holeDepth, rapidHeight, 3);
  g.spindleOff();
  g.close();
  dps.addAnnotation([&] {
      auto a = DebugAnnotation {
        DebugAnnotationDesc {
          "Guide holes",
          "Guide holes placed beyond the ends of the board."
        }
      };
      a.addSvgCircle(leftGuideHole(machine), holeDia);
      a.addSvgCircle(rightGuideHole(machine), holeDia);
      return a;
    });
  dps.addPath([&] {
      return DebugPath {
        m_overallPath,
        DebugAnnotationDesc {
          "Overall shape",
            "The final shape of the board, including edges.",
            "green", true
        }
      };
    });
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
  DebugPathSet& dps = addDebugPathSet("Core Alignment Marks");
  dps.addPath([&] {
      return DebugPath {
        buildCorePath(machine),
        DebugAnnotationDesc {
          "Core shape",
          "The final shape of the core, including sidewalls with overhang.",
          "green", true
        }
      };
    });
  auto a = DebugAnnotation {
    DebugAnnotationDesc {
      "Alignment Marks",
      "Small dimples placed on the bottom of the core which mark key locations "
      "and are useful for aligning the base to the core. "
      "Note: these are <b>very small</b> in the diagram, but they do "
      "accuratley represent the diameter of the dimple which will be left in "
      "the core. Zoom in!",
      "red"
    }
  };
  GCodeWriter g(m_name + "-core-alignment-marks.nc", tool,
                GCodeWriter::MaterialTop, GCodeWriter::YIsPartCenter,
                machine.normalSpeed(), machine.bottomRapidHeight());
  addCoreCenterComment(g);
  g.spindleOn();
  for (auto& p : marks) {
    g.rapidToPoint(p);
    g.feedToPoint(p);
    a.addSvgCircle(p, p.Z * -2);
  }
  g.rapidToPoint(marks.back());
  g.spindleOff();
  g.close();
  dps.addAnnotation([&] { return a; });
  return g;
}

//------------------------------------------------------------------------------
// Core edge groove

const GCodeWriter BoardShape::generateCoreEdgeGroove(const Machine& machine) {
  auto overallPath = buildOverallPath(machine);
  auto tool = machine.tool(machine.edgeGrooveTool());
  auto edgeWidth = machine.edgeGrooveEdgeWidth(); // Entire edge, not just tines
  auto grooveWidth = machine.sidewallOverhang() + edgeWidth;
  assert(grooveWidth >= tool.diameter);
  DebugPathSet& dps = addDebugPathSet("Edge Groove");
  dps.addDescription(
    "<p>The edge groove creates a rabbet along the perimeter of the core, "
    "leaving space for the edge material so the core still sits flat on the "
    "base material. It is created by machining a shallow trench %scm wide into "
    "the base of the core which matches precisely the edge of the core when it "
    "is finally cut out. Because of the sidewall overhang this ends up being "
    "slightly wider than the final rabbet width. The rabbet width should also "
    "be a bit wider than the exact edge material width to leave a little play "
    "when aligning the core to the base.</p>",
    grooveWidth.str().c_str());
  dps.addDescription(
    "<p>Since the width of the rabbet is typically larger than the cutter "
    "used, multiple machining passes are required.</p>");
  dps.addPath([&] {
      return DebugPath {
        buildCorePath(machine),
        DebugAnnotationDesc {
          "Core shape",
          "The final shape of the core, including sidewalls with overhang.",
          "green", true
        }
      };
    });
  dps.addPath([&] {
      return DebugPath {
        PathUtils::OffsetPath(overallPath, -edgeWidth)[0],
        DebugAnnotationDesc {
          "Groove outer edge",
          "The final, outer edge of the groove. This is a bit beyond the edge "
          "of the overall shape and matches the sidewall extension.",
          "orange", true
        }
      };
    });
  dps.addPath([&] {
      return DebugPath {
        PathUtils::OffsetPath(overallPath,
                              grooveWidth - edgeWidth)[0],
        DebugAnnotationDesc {
          "Groove inner edge",
          "The final, inner edge of the groove.",
          "orange", true
        }
      };
    });
  // Outer path is exact, inner path is exact. Build others inbetween
  // as appropriate.
  auto stepOffset = tool.diameter * machine.edgeGrooveOverlapPercentage();
  auto currentOffset = machine.sidewallOverhang() - (tool.diameter / 2);
  auto endOffset = -machine.edgeGrooveEdgeWidth() + (tool.diameter / 2);
  auto grooveDepth = machine.edgeGrooveDepth();
  vector<vector<Path>> groovePathSets;
  while (true) {
    auto paths = PathUtils::OffsetPath(overallPath, currentOffset);
    assert(paths.size() == 1);
    std::transform(paths[0].begin(), paths[0].end(), paths[0].begin(),
                   [&](const Point& p) { return Point(p.X, p.Y, grooveDepth); });
    groovePathSets.push_back(paths);
    dps.addPath([&] {
        return DebugPath {
          paths[0],
          DebugAnnotationDesc {
            "Edge groove toolpaths",
            "The toolpaths which will cut the edge groove."
          }
        };
      });
    if (currentOffset == endOffset) break;
    currentOffset -= stepOffset;
    if (currentOffset < endOffset) currentOffset = endOffset;
  }
  GCodeWriter g(m_name + "-core-edge-groove.nc", tool,
                GCodeWriter::MaterialTop, GCodeWriter::YIsPartCenter,
                machine.normalSpeed(), machine.bottomRapidHeight());
  addCoreCenterComment(g);
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
void emitBowl(GCodeWriter& g, MCFixed outerRimDia, MCFixed outerRimDepth) {
  vector<std::pair<MCFixed, MCFixed>> bowl {
    { outerRimDia, outerRimDepth },
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
  auto outerRimDia = machine.insertRimDiameter();
  auto outerRimDepth = machine.insertRimDepth();
  auto insertCenterHoleDia = machine.insertHoleDiameter();
  auto insertCenterHoleDepth = -machine.coreBlankThickness();
  g.comment("A single insert");
  g.commentF("  Outer rim diameter = %s\"",
    outerRimDia.inchesStr().c_str());
  g.commentF("  Outer rim depth    = %s\"",
    outerRimDepth.inchesStr().c_str());
  g.commentF("  Shaft diameter      = %s\"",
    insertCenterHoleDia.inchesStr().c_str());
  g.commentF("  Shaft depth         = %s\"",
    insertCenterHoleDepth.inchesStr().c_str());
  auto startPosition = g.currentPosition();
  if (heightAboveMaterial > 0) {
    g.feedToPoint(g.currentPosition() + Point(0, 0, -heightAboveMaterial));
  }
  auto holeCenter = g.currentPosition();
  emitBowl(g, outerRimDia, outerRimDepth);
  g.feedToPoint(holeCenter);
  g.emitIncrementalHole(insertCenterHoleDia, insertCenterHoleDepth, 0, 3);
  // Run the bowl again to clean it up.
  emitBowl(g, outerRimDia, outerRimDepth);
  g.rapidToPoint(startPosition);
}

const GCodeWriter BoardShape::generateInsertHoles(const Machine& machine) {
  if (m_insertsPath.size() == 0) {
    return GCodeWriter();
  }
  auto tool = machine.tool(machine.insertHolesTool());
  auto rapidHeight = machine.bottomRapidHeight();
  GCodeWriter g(m_name + "-core-insert-holes.nc", tool,
                GCodeWriter::MaterialTop, GCodeWriter::YIsPartCenter,
                machine.normalSpeed(), rapidHeight);
  addCoreCenterComment(g);
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

const GCodeWriter BoardShape::generateTopProfile(const Machine& machine,
                                                 BoardProfile& profile) {
  auto tool = machine.tool(machine.topProfileTool());
  // nb: use the overall path plus the sidewall overhang to limit the
  // profiling paths. This ensures the entire sidewall, even outside
  // the final edge, is profiled. It does mean we'll profile too much
  // on the nose and tail, since those are inset by the nose and tail
  // spacers, but that's an acceptable tradeoff for the simplicity of
  // this.
  auto offsetPaths = PathUtils::OffsetPath(buildOverallPath(machine),
                                           machine.sidewallOverhang());
  // nb: the roughing support is pretty experimental right now!!
#if ROUGHING
  bool roughing = machine.topProfileRoughing();
#else
  bool roughing = false; // @TODO: roughing is a work in-progress. Don't use it.
#endif
  assert(offsetPaths.size() == 1);
  auto overallOffset = offsetPaths[0];
  DebugPathSet& dps = addDebugPathSet("Top Profile");
  dps.addDescription("<p>These are the paths which apply the thickness profile "
                     "to the top of the core. They are joined inside-out, with "
                     "a lead-in of %s\" to the beginning of each island. A "
                     "%s\" cutter is used with an overlap of %.2f&#37;.</p>",
                     machine.topProfileLeadinLength().inchesStr().c_str(),
                     tool.diameter.inchesStr().c_str(),
                     machine.topProfileOverlapPercentage() * 100.0);
  if (roughing) {
    dps.addDescription("<p>Roughing paths are included, which limit the depth "
                       "of cut to reduce cutter load. They are overlapped with "
                       "the final thickness profile here.</p>");
  }
  dps.addPath([&] {
    return DebugPath {
      buildCorePath(machine),
      DebugAnnotationDesc {
        "Core shape",
        "The final shape of the core, including sidewalls with overhang.",
        "green", true
      }
    };
  });
  profile.debugPathSet().addPath([&] {
    return DebugPath {
      buildCorePath(machine),
      DebugAnnotationDesc {
        "Core shape",
        "The final shape of the core, including sidewalls with overhang.",
        "green", true
      }
    };
  });
  dps.addPath([&] {
    return DebugPath {
      overallOffset,
      DebugAnnotationDesc {
        "Outer profile pocket edge",
        "The outer edge of the pocket formed when machining the thickness "
        "profile. This is represents the limit of the material removed.",
        "orange", true
      }
    };
  });

  auto exp = [](const Path& p) { // Exaggerate a path in Y
    Path ep = p;
    std::transform(ep.begin(), ep.end(), ep.begin(),
                   [](const Point& p) { return Point(p.X, p.Y * 10); });
    return ep;
  };
  struct PathRange {
    MCFixed depth;
    Path::const_iterator begin;
    Path::const_iterator end;
  };
  typedef vector<PathRange> PathRanges;
  auto boxFromRange = [&](const PathRange& range) -> Path {
    auto x1 = (*range.begin).X;
    if (x1 == profile.path().begin()->X) {
      x1 -= 1;
    }
    auto x2 = (*(range.end - 1)).X;
    if (x2 == (profile.path().end() - 1)->X) {
      x2 += 1;
    }
    auto y = (maxWidth() / 2) + 1;
    Path p;
    p.push_back({x1, -y});
    p.push_back({x2, -y});
    p.push_back({x2, y});
    p.push_back({x1, y});
    p.push_back({x1, -y});
    return p;
  };

  vector<Path> roughingProfilePaths;
  vector<Path> roughingBoxes;

  if (roughing) {
    DebugPathSet& rdps = addDebugPathSet("Top Profile Roughing");
    rdps.addDescription("<p>Roughing passes for the core top profile. This "
                        "shows the depth for each roughing pass. All profile "
                        "paths are exaggerated by 10x.</p>");
    rdps.addPath([&] {
      return DebugPath {
        exp(profile.path()),
        DebugAnnotationDesc {
          "Final Profile",
          "The final profile path, exaggeragted 10x.",
          "red", true }
      };
    });
    rdps.addPath([&] {
      return DebugPath {
        overallOffset,
        DebugAnnotationDesc {
          "Offset overall", "Limit of material removed by top profiling.",
          "blue", true }
      };
    });

    MCFixed roughingOffset = machine.topProfileRoughingOffset();
    Path roughProf = PathUtils::OffsetOpenPath(profile.path(), roughingOffset);
    std::reverse(roughProf.begin(), roughProf.end()); // Keep it left-to-right

    auto roughLevel = [](const PathRange& range,
                         MCFixed upperLimit) -> PathRanges {
      PathRanges newRanges;
      bool rangeStarted = false;
      PathRange npr;
      npr.depth = upperLimit;
      for (auto i = range.begin; i != range.end; ++i) {
        if (i->Y < upperLimit) {
          if (!rangeStarted) {
            rangeStarted = true;
            npr.begin = i;
          }
        } else {
          if (rangeStarted) {
            rangeStarted = false;
            npr.end = i;
            newRanges.push_back(npr);
          }
        }
      }
      if (rangeStarted) {
        npr.end = range.end;
        newRanges.push_back(npr);
      }
      return newRanges;
    };

    // Build roughing profile paths and bounding boxes we can use to
    // clip the overall path later.
    std::stack<PathRange> rangeStack;
    rangeStack.push({ machine.coreBlankThickness(),
          roughProf.begin(), roughProf.end() });
    while (rangeStack.size() > 0) {
      auto r = rangeStack.top(); rangeStack.pop();
      auto newUpperLimit = r.depth - machine.topProfileRoughingMaxCutDepth();
      auto minPoint = std::min_element(r.begin, r.end,
                                       [](const Point& p1, const Point& p2) {
                                         return p1.Y < p2.Y;
                                       });
      if (newUpperLimit - minPoint->Y < machine.topProfileRoughingFuzz()) {
        newUpperLimit = minPoint->Y;
      }
      // Pull out the portion of the profile path within the range,
      // limiting its depth tothe new upper limit.
      Path rp;
      std::for_each(r.begin, r.end,
                    [&](const Point& p) {
                      rp.push_back({p.X, std::max(p.Y, newUpperLimit)});
                    });
      roughingProfilePaths.push_back(rp);
      auto roughingBox = boxFromRange(r);
      roughingBoxes.push_back(roughingBox);
      auto ep = exp(rp);
      rdps.addPath([&] {
        return DebugPath {
          ep,
          DebugAnnotationDesc { "Roughing Profile",
            "Each rouging profile. These show the depth of cut for each "
            "roughing pass, and are used to select and deform portions of the "
            "final profiling paths.", "purple", true }
        };
      });
      // Find new ranges below the path we just formed.
      for (auto r2 : roughLevel(r, newUpperLimit)) {
        rangeStack.push(r2);
      }
    }
  }
  // Finally, add on the real profile path and a box to contain it.
  roughingProfilePaths.push_back(profile.path());
  roughingBoxes.push_back(
    boxFromRange({ 0, profile.path().begin(), profile.path().end() }));

  vector<vector<vector<Path>>> cutGroups;
  for (int i = 0; i < roughingProfilePaths.size(); i++) {
    vector<vector<Path>> pathSets;
    auto& roughingProfile = roughingProfilePaths[i];
    auto& roughingBox = roughingBoxes[i];
    // Offset the profile path to account for the width of the cutter.
    auto profileOffset = ToolOffsetPath(roughingProfile, tool.diameter);
    // Offset the outer path until the offset disappears. nb: the first
    // path must be offset by half the tool diameter to ensure we follow
    // the limiting path correctly.
    double overlap = 0.5;
    for (int i = 1; i < 100; i++) { // Artifical upper limit
      auto resultPaths = PathUtils::OffsetPath(overallOffset,
                                               -tool.diameter * overlap * i);
      if (resultPaths.empty()) break; // All done!
      // Deform each path with the profile.
      vector<Path> deformedResults;
      for (auto& path : resultPaths) {
        Path& p = path;
        if (roughing) {
          auto cp = PathUtils::ClipPathsIntersect({path}, {roughingBox});
          if (cp.size() == 0) continue; // A bit on the other end, outside the box
          assert(cp.size() == 1);
          p = cp[0];
        }
        std::reverse(p.begin(), p.end());
        auto dp = ProfiledPath(p, profileOffset);
        deformedResults.push_back(dp);
        dps.addPath([&] {
          return DebugPath {
            dp,
            DebugAnnotationDesc {
              "Top profile path",
              "The paths used to profile the core.", "red", true
            }
          };
        });
      }
      pathSets.push_back(deformedResults);
      overlap = machine.topProfileOverlapPercentage(); // Use real overlap % now
    }
    cutGroups.push_back(pathSets);
  }
  auto rapidHeight = machine.topRapidHeight();;
  GCodeWriter g(m_name + "-top-profile.nc", tool,
                GCodeWriter::TableTop, GCodeWriter::YIsPartCenter,
                machine.topProfileDeepSpeed(), rapidHeight);
  addCoreCenterComment(g);
  g.spindleOn();
  for (auto& pathSets : cutGroups) {
    g.emitPathSets(pathSets, true, rapidHeight,
                   machine.topProfileLeadinLength(),
                   machine.topProfileTransitionSpeed());
  }
  g.spindleOff();
  g.close();
  return g;
}

//------------------------------------------------------------------------------
// Nose and tail spacer cutouts

const GCodeWriter BoardShape::generateNoseTailSpacerCutout(
  const Machine& machine)
{
  DebugPathSet& dps = addDebugPathSet("Nose Tail Spacers");
  dps.addDescription(
    "<p>Nose and tail spacers are thin sections of PTEX which act as sidewalls "
    "along the nose and tail to protect the core. They are typically a bit "
    "wider than normal sidewalls to act as a bit more of a 'bumper' against "
    "impact. The spacers are cut wider and longer than necessary to ensure "
    "they overlap the edges well, and for simplicity.</p>");
  auto tool = machine.tool(machine.baseCutoutTool());
  auto nosePath = m_noseSpacerPath;
  auto tailPath = m_tailSpacerPath;
  dps.addPath([&] {
      return DebugPath {
        nosePath,
        DebugAnnotationDesc {
          "Nose spacer",
          "The shape of the nose spacer. Note that we only cut the curve which "
          "interfaces with the core. The rest of the shape depends on the "
          "length and width of your material.",
          "orange", true
        }
      };
    });
  dps.addPath([&] {
      return DebugPath {
        tailPath,
        DebugAnnotationDesc {
          "Tail spacer",
          "The shape of the tail spacer. Note that we only cut the curve which "
          "interfaces with the core. The rest of the shape depends on the "
          "length and width of your material.",
          "orange", true
        }
      };
    });
  dps.addPath([&] {
      return DebugPath {
        buildCorePath(machine),
        DebugAnnotationDesc {
          "Core shape",
          "The final shape of the core, including sidewalls with overhang.",
          "green", true
        }
      };
    });

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

  dps.addPath([&] {
      return DebugPath {
        onp,
        DebugAnnotationDesc {
          "Nose cut path",
          "The path used to cut the nose spacer."
        }
      };
    });
  dps.addPath([&] {
      return DebugPath {
        otp,
        DebugAnnotationDesc {
          "Tail cut path",
          "The path used to cut the tail spacer."
        }
      };
    });

  // Shift the nose right so it starts at X=0.
  auto nShift = -nosePath[0].X;
  assert(nShift >= 0);
  std::transform(onp.begin(), onp.end(), onp.begin(),
                 [&](const Point& p) { return p + Point(nShift, 0); });

  // Shift the tail left so it comes within 2cm of the nose path.
  auto nmax = std::max_element(onp.begin(), onp.end(),
                               [](const Point& a, const Point& b) {
                                 return a.X < b.X;
                               });
  auto tShift = nmax->X + 2 - otp[0].X;
  assert(tShift <= 0);
  std::transform(otp.begin(), otp.end(), otp.begin(),
                 [&](const Point& p) { return p + Point(tShift, 0); });

  GCodeWriter g(m_name + "-nose-tail-spacers.nc", tool,
                GCodeWriter::TableTop, GCodeWriter::YIsPartCenter,
                machine.normalSpeed(), machine.baseRapidHeight());
  auto materialLength = m_noseLength + m_tailLength + 2 +
    machine.spacerEndOverhang() * 2;
  addCoreCenterComment(g);
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
  // The outer edge of the edge trench is at the sidewall overhang.
  // We need to move the edge of the overall shape to the center of
  // the edge trench so we can offset it evenly on each side.
  DebugPathSet& dps = addDebugPathSet("Edge Trench");
  auto& overallPath = buildOverallPath(machine);
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
  // cutter radius to leave room for the rounded corners on both
  // ends.
  //
  // @TODO: older versions of MonkeyCAM used to compute the
  // intersection with the core path, then extend from there, so older
  // extension values were quite a bit shorter. This is preferable,
  // and I should adjust this to do the same one day.
  auto tool = machine.tool(machine.coreCutoutTool());
  extendLine(etUpperCenter, machine.edgeTrenchExtension() +
             tool.diameter);
  extendLine(etLowerCenter, machine.edgeTrenchExtension() +
             tool.diameter);

  // Form the trench paths by offseting the two centerlines.
  auto trenches =
    PathUtils::OffsetLines(vector<Path> { etLowerCenter, etUpperCenter},
                           machine.edgeTrenchWidth() / 2);
  assert(trenches.size() == 2);

  // Offset the trenches inwards for machining.
  auto ps = PathUtils::OffsetPath(trenches[0], -tool.diameter / 2);
  assert(ps.size() == 1);
  auto t1 = ps[0];
  std::reverse(t1.begin(), t1.end());
  ps = PathUtils::OffsetPath(trenches[1], -tool.diameter / 2);
  assert(ps.size() == 1);
  auto t2 = ps[0];
  std::reverse(t2.begin(), t2.end());

  dps.addDescription(
    "<p>The edge trenches provide space in which to place a different type "
    "of wood along the effective edge, as well as sidewall material. The outer "
    "edge of the trenches matches the edge of the core precisely, including "
    "the sidewall overhang. The trench is %scm wide and extends %scm past the "
    "ends of the effective edge.</p>",
    machine.edgeTrenchWidth().str().c_str(),
    (machine.edgeTrenchExtension() + tool.diameter).str().c_str()
  );
  dps.addPath([&] {
      return DebugPath {
        t1,
        DebugAnnotationDesc {
          "Edge Trench Path",
          "The path used to cut the edge trench."
        }
      };
    });
  dps.addPath([&] {
      return DebugPath {
        t2,
        DebugAnnotationDesc {
          "Edge Trench Path",
          "The path used to cut the edge trench."
        }
      };
    });
  dps.addPath([&] {
      return DebugPath {
        trenches[0],
        DebugAnnotationDesc {
          "Edge Trench",
          "The final shapre of the edge trench.",
          "blue", true
        }
      };
    });
  dps.addPath([&] {
      return DebugPath {
        trenches[1],
        DebugAnnotationDesc {
          "Edge Trench",
          "The final shapre of the edge trench.",
          "blue", true
        }
      };
    });
  dps.addPath([&] {
      return DebugPath {
        buildCorePath(machine),
        DebugAnnotationDesc {
          "Core shape",
          "The final shape of the core, including sidewalls with overhang.",
          "orange", true
        }
      };
    });

  GCodeWriter g(m_name + "-edge-trench.nc", tool,
                GCodeWriter::TableTop, GCodeWriter::YIsPartCenter,
                machine.normalSpeed(), machine.topRapidHeight());
  addCoreCenterComment(g);
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
  DebugPathSet& dps = addDebugPathSet("Core Top Cutout");
  // Offset the core path as usual
  auto tool = machine.tool(machine.coreCutoutTool());
  auto paths = PathUtils::OffsetPath(buildCorePath(machine),
                                     tool.diameter / 2);
  assert(paths.size() == 1);
  auto op = paths[0];
  dps.addPath([&] {
      return DebugPath {
        op,
        DebugAnnotationDesc {
          "Core top cutout path",
          "The path used to cut the final core."
        }
      };
    });
  dps.addPath([&] {
      return DebugPath {
        buildCorePath(machine),
        DebugAnnotationDesc {
          "Core shape",
          "The final shape of the core, including sidewalls with overhang.",
          "orange", true
        }
      };
    });

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
  addCoreCenterComment(g);
  g.rapidToPoint(profPath[0]);
  g.spindleOn();
  g.emitSpiralPath(profPath, machine.coreBlankThickness(),
                   machine.coreCutoutPasses());
  g.rapidToPoint(profPath[0]);
  g.spindleOff();
  g.close();
  return g;
}

} // namespace MonkeyCAM
