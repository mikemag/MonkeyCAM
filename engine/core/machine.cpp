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

#include "machine.h"

#include <cstdio>
#include <cstdlib>
#include <map>
#include <optional>
#include <string>

#include "config.h"

namespace MonkeyCAM {

//------------------------------------------------------------------------------
// Machine

Machine::Machine(Config& config) : m_config(config) {
  auto machine = m_config.get<json>("machine", Config::validateIsObject);
  m_config.push("machine", machine);
  m_coreCutoutPasses = readPassCount("core cutout passes");
  m_edgeTrenchPasses = readPassCount("edge trench passes");
  m_topProfilePasses = readPassCount("top profile passes");
  auto units = m_config.get<std::string>("gcode units", std::string("in"));
  if (units == "mm") {
    m_gcodeUnits = GCodeUnits::Millimeters;
  } else {
    m_gcodeUnits = GCodeUnits::Inches;
  }
  auto tools = m_config.get<json>("tools", Config::validateIsArray);
  m_config.push("tools", tools);

  int toolNum = 0;
  for (auto& tool : tools) {
    m_config.push(("[" + std::to_string(toolNum++) + "]").c_str(), tool);
    auto id = m_config.get<int>("id");
    auto name = m_config.get<std::string>("name");
    auto dia = m_config.getLength("diameter", LengthUnit::Inches);
    auto gcodeNum = m_config.get<int>("G-code #");
    std::optional<MCFixed> maxCutDepth;
    auto configuredMax =
        m_config.getOptionalLength("max cut depth", LengthUnit::Inches);
    if (configuredMax) {
      maxCutDepth = MCFixed(*configuredMax);
      if (*maxCutDepth <= 0) failNonPositive("max cut depth");
    }
    m_tools.insert({id, {name, dia, gcodeNum, maxCutDepth}});
    m_config.pop();
  }
  m_config.pop();
  // NB: leaving 'machine' pushed for the accessors later.
}

int Machine::readPassCount(const char* key) {
  auto passes = m_config.get<int>(key, 1);
  if (passes <= 0) failNonPositive(key);
  return passes;
}

void Machine::failNonPositive(const char* key) {
  printf("Invalid parameter in Machine Configuration: '%s' must be greater "
         "than zero\n",
         key);
  ae::emitter().write(
      {{"error",
        {{"kind", "Invalid Parameter"},
         {"configName", "Machine Configuration"},
         {"key", key},
         {"message", "value must be greater than zero"}}}});
  std::exit(1);
}

}  // namespace MonkeyCAM
