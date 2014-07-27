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

#include "svg-writer.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <cassert>

#include "../core/paths.h"

namespace MonkeyCAM {

SVGWriter::SVGWriter(std::string filename, int yShift, double scale)
    : m_outputStream(filename)
    , m_yShift(yShift)
    , m_scale(scale) {
  std::ifstream base("grid.svg");
  assert(base.is_open());
  std::string line;
  while (base.good()) {
    getline(base, line);
    m_outputStream << line << std::endl;
  }
  base.close();
}

SVGWriter::~SVGWriter() {
  if (m_outputStream.is_open()) {
    m_outputStream << "</svg>" << std::endl;
    m_outputStream.close();
  }
}

void SVGWriter::addPath(const Path& path, std::string color, std::string desc) {
  assert(m_outputStream.is_open());
  m_outputStream << "<polyline points=\"";
  for (auto const& p : path) {
    double x = (p.X.dbl() + 3) * m_scale;
    double y = (m_yShift - p.Y.dbl()) * m_scale;
    m_outputStream << x << "," << y << " ";
  }
  m_outputStream << "\" style=\"fill-opacity:0.5;fill:rgb(0,0,0);"
    "stroke:" << color << ";stroke-width:1;fill:none\"/>" << std::endl;
  auto fp = path[0];
  m_outputStream << "<circle cx=\"" << (fp.X.dbl() + 3) * m_scale <<
    "\" cy=\"" << (m_yShift - fp.Y.dbl()) * m_scale <<
    "\" r=\"2\" style=\"stroke-width:1;stroke:red;fill:none\"/>" << std::endl;
}

void SVGWriter::addPointMark(const Point& p, std::string color) {
  m_outputStream << "<circle cx=\"" << (p.X.dbl() + 3) * m_scale <<
    "\" cy=\"" << (m_yShift - p.Y.dbl()) * m_scale <<
    "\" r=\"2\" style=\"stroke-width:1;stroke:" << color <<
    ";fill:none\"/>" << std::endl;
}

} // namespace MonkeyCAM
