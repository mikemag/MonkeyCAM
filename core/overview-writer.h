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

#include <iostream>
#include <fstream>
#include <functional>

#include "paths.h"

namespace MonkeyCAM {

class OverviewWriter {
 public:
  OverviewWriter(std::string filename, std::string name);
  ~OverviewWriter();

  void addHeader(const std::string header, const std::string headerLink = "");
  void addRaw(const char* raw);
  void addFormatted(const char* fmt, ...);

  void startDrawing(int width, int height);
  void endDrawing();
  void addPath(const Path& path, std::string color = "black",
               bool dashed = false);
  void addAnnotation(const DebugAnnotation& annotation);

  void addCode(std::function<void(std::ofstream&)> emitter);

 private:
  std::ofstream m_outputStream;
};

} // namespace MonkeyCAM
