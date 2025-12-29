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

#ifndef incl_machine_H_
#define incl_machine_H_

#include <map>
#include <string>

#include "config.h"
#include "mcfixed.h"

namespace MonkeyCAM {

enum class GCodeUnits { Inches, Millimeters };

//------------------------------------------------------------------------------
// Tool
//
// Defines a tool used by a CNC machine.

struct Tool {
  Tool() : name(""), diameter(0), gcodeToolNumber(0) {}
  Tool(std::string name, MCFixed diameter, int toolNumber)
      : name(name), diameter(diameter), gcodeToolNumber(toolNumber) {}

  std::string name;
  MCFixed diameter;
  int gcodeToolNumber;
};

//------------------------------------------------------------------------------
// Machine
//
// Holds parameters related to the CNC machine that will cut the parts
// modeled by MonkeyCAM.

class Machine {
 public:
  Machine(Config& config);

  const Tool& tool(int id) const { return m_tools.at(id); }
  GCodeUnits gcodeUnits() const { return m_gcodeUnits; }

#define MPT(_f, _t, _n) \
  const _t _f() const { return m_config.get<_t>(_n); }
#define MPI(_f, _n)                                     \
  const MCFixed _f() const {                            \
    return m_config.getLength(_n, LengthUnit::Inches);  \
  }

  MPT(rapidSpeed, int, "rapid speed")
  MPT(normalSpeed, int, "normal speed")
  MPI(bottomRapidHeight, "bottom rapid height")
  MPI(topRapidHeight, "top rapid height")
  MPI(coreBlankThickness, "core blank thickness")
  MPT(baseCutoutTool, int, "base cutout tool")
  MPI(baseRapidHeight, "base rapid height")
  MPI(baseCutThruHeight, "base cut thru height")
  MPT(guideHoleTool, int, "guide hole tool")
  MPI(guideHoleDepth, "guide hole depth")
  MPI(guideHoleDiameter, "guide hole diameter")
  MPI(guideHoleOffset, "guide hole offset")
  MPT(alignmentMarkTool, int, "alignment mark tool")
  MPI(alignmentMarkOffset, "alignment mark offset")
  MPI(alignmentMarkDepth, "alignment mark depth")
  MPI(alignmentMarkDeepDepth, "alignment mark deep depth")
  MPT(edgeGrooveTool, int, "edge groove tool")
  MPI(edgeGrooveDepth, "edge groove depth")
  MPI(edgeGrooveEdgeWidth, "edge groove edge width")
  MPT(edgeGrooveOverlapPercentage, double, "edge groove overlap percentage")
  MPT(insertHolesTool, int, "insert tool")
  MPI(insertRimDepth, "insert rim depth")
  MPI(insertRimDiameter, "insert rim diameter")
  MPI(insertHoleDiameter, "insert hole diameter")
  MPT(topProfileTool, int, "top profile tool")
  MPT(topProfileTransitionSpeed, int, "top profile transition speed")
  MPT(topProfileDeepSpeed, int, "top profile deep speed")
  MPT(topProfileOverlapPercentage, double, "top profile overlap percentage")
  MPI(topProfileLeadinLength, "top profile lead-in length")
  MPT(topProfileRoughing, bool, "top profile roughing")
  MPI(topProfileRoughingMaxCutDepth, "top profile roughing max cut depth")
  MPI(topProfileRoughingOffset, "top profile roughing offset")
  MPI(topProfileRoughingFuzz, "top profile roughing fuzz")
  MPT(coreCutoutTool, int, "core cutout tool")
  MPT(coreCutoutPasses, int, "core cutout passes")
  MPI(spacerEndOverhang, "spacer end overhang")
  MPI(spacerSideOverhang, "spacer side overhang")
  MPI(sidewallOverhang, "sidewall overhang")
  MPI(edgeTrenchWidth, "edge trench width")
  MPI(edgeTrenchExtension, "edge trench extension")
  MPI(splitboardCenterTrenchWidth, "splitboard center trench width")
  MPI(splitboardCenterGap, "splitboard center gap")
#undef MPT
#undef MPI

 private:
  Config& m_config;
  /*const*/ std::map<int, const Tool> m_tools;
  GCodeUnits m_gcodeUnits;
};

}  // namespace MonkeyCAM

#endif  // incl_machine_H_
