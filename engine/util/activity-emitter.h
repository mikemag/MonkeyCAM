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

#ifndef incl_activity_emitter_H_
#define incl_activity_emitter_H_

#include <boost/optional/optional.hpp>
#include "json.hpp"
using json = nlohmann::json;

namespace MonkeyCAM {

class ActivityEmitter {
 public:
  static void initialize(boost::optional<int> fd);
  static ActivityEmitter& emitter() { return *m_emitter; }

  void write(const json& obj);
  void write(const char* key, const char* fmt, ...);
  void fatal(const char* fmt, ...);

 private:
  ActivityEmitter(boost::optional<int> fd);

  static ActivityEmitter* m_emitter;

  int m_jsonOutputFD;
  FILE* m_jsonOutputFile;
};

} // namespace MonkeyCAM

#endif // incl_activity_emitter_H_
