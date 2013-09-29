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

#ifndef incl_machine_H_
#define incl_machine_H_

#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>

#include "mcfixed.h"

namespace MonkeyCAM {

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
  Machine(boost::property_tree::ptree& config);

  const Tool& tool(int id) { return m_tools[id]; }

#define MPT(_f, _t, _n) _t _f() { return m_config.get<_t>("machine." _n); }
#define MPI(_f, _n) MCFixed _f() {                                 \
  return MCFixed::fromInches(m_config.get<double>("machine." _n)); \
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
  MPT(alignmentMarkTool, int, "alignment mark tool")
  MPI(alignmentMarkOffset, "alignment mark offset")
  MPI(alignmentMarkDepth, "alignment mark depth")
  MPI(alignmentMarkDeepDepth, "alignment mark deep depth")
  MPT(edgeGrooveTool, int, "edge groove tool")
  MPI(edgeGrooveDepth, "edge groove depth")
  MPT(insertHolesTool, int, "insert tool")
  MPI(insertRimDepth, "insert rim depth")
  MPI(insertRimDiameter, "insert rim diameter")
  MPI(insertHoleDiameter, "insert hole diameter")
  MPT(topProfileTool, int, "top profile tool")
  MPT(topProfileTransitionSpeed, int, "top profile transition speed")
  MPT(topProfileDeepSpeed, int, "top profile deep speed")
  MPT(topProfileOverlapPercentage, double, "top profile overlap percentage")
  MPI(topProfileLeadinLength, "top profile lead-in length")
  MPT(coreCutoutTool, int, "core cutout tool")
  MPI(spacerEndOverhang, "spacer end overhang")
  MPI(spacerSideOverhang, "spacer side overhang")
  MPI(sidewallOverhang, "sidewall overhang")
#undef MPT
#undef MPI

 private:
  boost::property_tree::ptree m_config;
  std::map<int, Tool> m_tools;
};

} // namespace MonkeyCAM

#endif // incl_machine_H_
