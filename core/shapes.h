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

#ifndef incl_shapes_H_
#define incl_shapes_H_

#include <cassert>
#include <string>
#include <memory>

#include "board-profile.h"
#include "gcode-writer.h"
#include "insert-pack.h"
#include "paths.h"
#include "shape-parts.h"

namespace MonkeyCAM {

//------------------------------------------------------------------------------

class BoardShape {
 public:
  BoardShape(std::string name,
             MCFixed noseLength, MCFixed effectiveEdge, MCFixed tailLength,
             MCFixed sidecutRadius, MCFixed waistWidth, MCFixed taper,
             std::unique_ptr<ShapeEndPart>& nosePart,
             std::unique_ptr<ShapeEdgePart>& edgePart,
             std::unique_ptr<ShapeEndPart>& tailPart,
             MCFixed refStance, MCFixed setback,
             std::unique_ptr<InsertPack>& nosePack,
             std::unique_ptr<InsertPack>& tailPack,
             MCFixed spacerWidth);
  const Path& buildOverallPath();
  const Path& buildCorePath(Machine& machine);

  const GCodeWriter generateBaseCutout(Machine& machine);
  const GCodeWriter generateNoseTailSpacerCutout(Machine& machine);
  const GCodeWriter generateCoreAlignmentMarks(Machine& machine);
  const GCodeWriter generateGuideHoles(Machine& machine);
  const GCodeWriter generateCoreEdgeGroove(Machine& machine);
  const GCodeWriter generateEdgeTrench(Machine& machine);
  const GCodeWriter generateInsertHoles(Machine& machine);
  const GCodeWriter generateTopProfile(Machine& machine,
                                       BoardProfile profile);
  const GCodeWriter generateTopCutout(Machine& machine);

  const Path& insertsPath() const { return m_insertsPath; }
  MCFixed noseLength() const { return m_noseLength; }
  MCFixed eeLength() const { return m_effectiveEdge; }
  MCFixed overallLength() const { return m_noseLength + m_tailLength +
      m_effectiveEdge; }
  const std::string name() const { return m_name; }

 private:
  void setupInserts();
  void addInsertPack(InsertPack& pack, Point center);

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

  MCFixed m_setback;
  MCFixed m_refStance;
  std::unique_ptr<InsertPack> m_noseInserts;
  std::unique_ptr<InsertPack> m_tailInserts;
  Path m_insertsPath;

  MCFixed m_spacerWidth;

  Path m_overallPath;
  Path m_corePath;
  Path m_noseSpacerPath;
  Path m_tailSpacerPath;

  MCFixed m_maxCoreX;

  Point m_leftGuideHole;
  Point m_rightGuideHole;
};

} // namespace MonkeyCAM

#endif // incl_shapes_H_
