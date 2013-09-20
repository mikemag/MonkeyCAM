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

#include <iostream>
#include <fstream>

namespace MonkeyCAM {

class Path;

class SVGWriter {
 public:
  SVGWriter(std::string filename, int yShift = 5, double scale = 100.0);
  ~SVGWriter();
  void addPath(Path const& path, std::string color = "rgb(0,0,0)");

 private:
  std::ofstream m_outputStream;
  int m_yShift;
  double m_scale;
};

} // namespace MonkeyCAM
