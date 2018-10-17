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
#include "config.h"

#include <map>
#include <string>

namespace MonkeyCAM {

//------------------------------------------------------------------------------
// Machine

Machine::Machine(Config& config) : m_config(config) {
  auto machine = m_config.get<json>("machine", Config::validateIsObject);
  m_config.push("machine", machine);
  auto tools = m_config.get<json>("tools", Config::validateIsArray);
  m_config.push("tools", tools);

  int toolNum = 0;
  for (auto& tool : tools) {
    m_config.push(("[" + std::to_string(toolNum++) + "]").c_str(), tool);
    auto id = m_config.get<int>("id");
    auto name = m_config.get<std::string>("name");
    auto dia = MCFixed::fromInches(m_config.get<double>("diameter"));
    auto gcodeNum = m_config.get<int>("G-code #");
    m_tools.insert({id, {name, dia, gcodeNum}});
    m_config.pop();
  }
  m_config.pop();
  // NB: leaving 'machine' pushed for the accessors later.
}

}  // namespace MonkeyCAM
