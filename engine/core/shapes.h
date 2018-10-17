/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
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

#ifndef incl_shapes_H_
#define incl_shapes_H_

#include <boost/optional/optional.hpp>
#include <cassert>
#include <memory>
#include <string>

#include "board-profile.h"
#include "gcode-writer.h"
#include "insert-pack.h"
#include "paths.h"
#include "shape-parts.h"

namespace MonkeyCAM {

//------------------------------------------------------------------------------

class BoardShape {
 public:
  BoardShape(std::string name, MCFixed noseLength, MCFixed effectiveEdge,
             MCFixed tailLength, MCFixed sidecutRadius, MCFixed waistWidth,
             MCFixed taper, std::unique_ptr<ShapeEndPart>& nosePart,
             std::unique_ptr<ShapeEdgePart>& edgePart,
             std::unique_ptr<ShapeEndPart>& tailPart,
             boost::optional<MCFixed> refStance,
             boost::optional<MCFixed> setback, MCFixed bindingDist,
             std::unique_ptr<InsertPack>& nosePack,
             std::unique_ptr<InsertPack>& tailPack,
             std::unique_ptr<InsertPack>& toeInserts,
             std::unique_ptr<InsertPack>& centerInserts,
             std::unique_ptr<InsertPack>& heelInserts, MCFixed spacerWidth,
             boost::optional<MCFixed> noseEdgeExt,
             boost::optional<MCFixed> tailEdgeExt, bool isSplitboard);
  ~BoardShape();
  const Path& buildOverallPath(const Machine& machine);
  const Path& buildCorePath(const Machine& machine);

  const GCodeWriter generateBaseCutout(const Machine& machine);
  const GCodeWriter generateNoseTailSpacerCutout(const Machine& machine);
  const Path alignmentMarksPath(const Machine& machine);
  const GCodeWriter generateCoreAlignmentMarks(const Machine& machine);
  const GCodeWriter generateGuideHoles(const Machine& machine);
  const GCodeWriter generateCoreEdgeGroove(const Machine& machine);
  const GCodeWriter generateEdgeTrench(const Machine& machine);
  const GCodeWriter generateSplitboardCenterTrench(const Machine& machine);
  const GCodeWriter generateInsertHoles(const Machine& machine);
  const GCodeWriter generateTopProfile(const Machine& machine,
                                       BoardProfile& profile);
  const GCodeWriter generateTopCutout(const Machine& machine);

  const Path& insertsPath() const { return m_insertsPath; }
  MCFixed noseLength() const { return m_noseLength; }
  MCFixed eeLength() const { return m_effectiveEdge; }
  MCFixed overallLength() const {
    return m_noseLength + m_tailLength + m_effectiveEdge;
  }
  MCFixed maxWidth() const {
    return std::max(std::max(m_noseWidth, m_waistWidth), m_tailWidth);
  }
  MCFixed minWidth() const {
    return std::min(std::min(m_noseWidth, m_waistWidth), m_tailWidth);
  }
  const std::string name() const { return m_name; }

  const std::vector<DebugPathSet*>& debugPathSets() const {
    return m_debugPathSets;
  }

  const bool isSplitboard() const { return m_isSplitboard; }

 private:
  void setupInserts();
  void addInsertPack(InsertPack& pack, Point center);
  const Point leftGuideHole(const Machine& machine) const;
  const Point rightGuideHole(const Machine& machine) const;

  DebugPathSet& addDebugPathSet(std::string header);

  const void addCoreCenterComment(GCodeWriter& g);

  const Path spreadPathForSplitboards(const Path& p, const Machine& machine);
  const Point spreadPointForSplitboards(const Point& p, const Machine& machine);

  std::string m_name;

  MCFixed m_noseLength;
  MCFixed m_effectiveEdge;
  MCFixed m_tailLength;

  MCFixed m_noseWidth;
  MCFixed m_waistWidth;
  MCFixed m_tailWidth;
  MCFixed m_sidecutRadius;
  MCFixed m_sidecutDepth;
  MCFixed m_taper;
  std::unique_ptr<ShapeEndPart> m_nosePart;
  std::unique_ptr<ShapeEdgePart> m_edgePart;
  std::unique_ptr<ShapeEndPart> m_tailPart;

  boost::optional<MCFixed> m_setback;
  boost::optional<MCFixed> m_refStance;
  MCFixed m_bindingDist;
  std::unique_ptr<InsertPack> m_noseInserts;
  std::unique_ptr<InsertPack> m_tailInserts;
  std::unique_ptr<InsertPack> m_toeInserts;
  std::unique_ptr<InsertPack> m_centerInserts;
  std::unique_ptr<InsertPack> m_heelInserts;
  Path m_insertsPath;

  MCFixed m_spacerWidth;

  boost::optional<MCFixed> m_noseEdgeExt;
  boost::optional<MCFixed> m_tailEdgeExt;

  bool m_isSplitboard;

  Path m_overallPath;
  Path m_corePath;
  Path m_noseSpacerPath;
  Path m_tailSpacerPath;

  MCFixed m_maxCoreX;

  std::vector<DebugPathSet*> m_debugPathSets;
};

}  // namespace MonkeyCAM

#endif  // incl_shapes_H_
