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

#include "machine.h"

#include <map>
#include <boost/property_tree/ptree.hpp>
#include <string>

namespace MonkeyCAM {

//------------------------------------------------------------------------------
// Machine

Machine::Machine(const boost::property_tree::ptree& config)
    : m_config(config)
{
  for (auto& tool : config.get_child("machine.tools")) {
    auto id = tool.second.get<int>("id");
    auto name = tool.second.get<std::string>("name");
    auto dia = MCFixed::fromInches(tool.second.get<double>("diameter"));
    auto gcodeNum = tool.second.get<int>("G-code #");
    m_tools.insert({ id, { name, dia, gcodeNum } });
  }
}

} // namespace MonkeyCAM
