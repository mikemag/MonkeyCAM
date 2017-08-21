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

#include "config.h"

#include <iostream>
#include <fstream>

#include "activity-emitter.h"
using ae = MonkeyCAM::ActivityEmitter;
#include "json.hpp"
using json = nlohmann::json;

using std::string;

namespace MonkeyCAM {

Config::Config(const string configName,
               const string fileName
              ) :
    m_configName(configName),
    m_fileName(fileName)
{
  if (m_fileName.empty()) {
    m_stack.push({"", m_rootJson});
    return;
  }
  std::ifstream f(m_fileName.c_str());
  try {
    f >> m_rootJson;
    f.close();
    m_stack.push({"", m_rootJson});
  } catch (json::parse_error& e) {
    string what = e.what();
    string msg = what.substr(what.find(':') + 2, string::npos);
    f.seekg(0);
    auto offset = e.byte;
    auto lineNumber = 1;
    auto remainingOffset = e.byte;
    char c;
    while (f.get(c) && --offset) {
      if (c == '\n') {
        lineNumber++;
        remainingOffset = offset;
      }
    }
    printf("Parse error in %s on line %d character %lu: %s\n",
           fileName.c_str(), lineNumber, remainingOffset, msg.c_str());
    ae::emitter().write({
        {"error", {
            {"kind", "Parse Error"},
            {"configName", configName},
            {"lineNumber", lineNumber},
            {"offset", remainingOffset},
            {"message", msg}
          }
        }
      });
    exit(1);
  }
}

/* static */ void Config::validateIsArray(const json& j) {
  if (!j.is_array()) {
    throw json::type_error::create(
      302, "type must be array, but is " + string(j.type_name()));
  }
}

/* static */ void Config::validateIsObject(const json& j) {
  if (!j.is_object()) {
    throw json::type_error::create(
      302, "type must be object, but is " + string(j.type_name()));
  }
}

} // namespace MonkeyCAM
