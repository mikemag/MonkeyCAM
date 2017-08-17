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

#include <map>
#include <string>

namespace MonkeyCAM {

//------------------------------------------------------------------------------
// Machine

Machine::Machine(const json& config)
    : m_config(config)
{
  m_machine = m_config.at("machine");              \

  for (auto& tool : m_machine.at("tools")) {
    int id = tool.at("id");
    std::string name = tool.at("name");
    auto dia = MCFixed::fromInches(tool.at("diameter").get<double>());
    int gcodeNum = tool.at("G-code #");
    m_tools.insert({ id, { name, dia, gcodeNum } });
  }
}

} // namespace MonkeyCAM
