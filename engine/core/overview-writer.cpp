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

#include "overview-writer.h"

#include <stdarg.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>

namespace MonkeyCAM {

OverviewWriter::OverviewWriter(std::string filename, std::string name)
    : m_outputStream(filename) {
  assert(m_outputStream.is_open());
  m_outputStream << "<!DOCTYPE html>" << std::endl;
  m_outputStream << "<html lang=\"en\">" << std::endl;
  m_outputStream << "<head><meta charset=\"UTF-8\"><title>Overview of " << name << "</title>"
                 << std::endl;
  m_outputStream
      << "<style>"
         R"(body {font-family:"arial";})"
         R"(#generated {font-style:italic;font-size:80%;})"
         R"(#disclaimer {margin:0;border:1px solid rgb(200,0,0);)"
         R"(background-color:rgba(255,0,0,0.1);padding:0.5em;font-size:80%;})"
         R"(.pathname {font-weight:bold;})"
         "</style>"
      << std::endl;
  m_outputStream << "</head><body>" << std::endl;
}

OverviewWriter::~OverviewWriter() {
  if (m_outputStream.is_open()) {
    m_outputStream << "</body></html>" << std::endl;
    m_outputStream.close();
  }
}

void OverviewWriter::addHeader(const std::string header,
                               const std::string headerLink) {
  assert(m_outputStream.is_open());
  std::string id(headerLink);
  if (headerLink == "") {
    id = header;
    std::replace(id.begin(), id.end(), ' ', '_');
  }
  m_outputStream << "<h2><a id=\"" << id << "\">" << header << "</a></h2>"
                 << std::endl;
}

void OverviewWriter::addRaw(const char* html) {
  assert(m_outputStream.is_open());
  m_outputStream << html << std::endl;
}

void OverviewWriter::addFormatted(const char* fmt, ...) {
  assert(m_outputStream.is_open());
  va_list args;
  va_start(args, fmt);
  char buff[8192];
  vsnprintf(buff, sizeof(buff), fmt, args);
  m_outputStream << buff << std::endl;
  va_end(args);
}

void OverviewWriter::startDrawing(int width, int height) {
  assert(m_outputStream.is_open());
  const int widthExtra = 10;
  const int heightExtra = 10;
  const int scale = 8;  // @todo: program option?
  width += widthExtra;
  height += heightExtra;
  if ((height / 2) * 2 != height) {
    height++;  // Need the height to be even so the entire grid gets filled out.
  }
  addFormatted(R"(<svg xmlns="http://www.w3.org/2000/svg")"
               R"( width="%dpx" height="%dpx" viewBox="%d %d %d %d">)",
               width * scale + 20,  // +20 for a little whitespace on the right
               height * scale, -widthExtra / 2, -height / 2, width, height);
  addFormatted(R"(<rect x="%d" y="%d" width="%d" height="%d" fill="yellow")"
               R"( stroke="blue" stroke-width="0.2" fill-opacity="0.05")"
               R"( stroke-opacity="0.9"></rect>)",
               -widthExtra / 2, -height / 2, width, height);
  addFormatted(R"(<g stroke-opacity="0.33" stroke-width="0.025")"
               R"( stroke="rgb(128,128,128) ">)");
  // Grid
  auto gridLine = [&](int i, int x1, int x2, int y1, int y2) {
    const char* width = i % 10 == 0 ? " stroke-width=\"0.075\"" : "";
    addFormatted(R"(<line x1="%d" y1="%d" x2="%d" y2="%d"%s></line>)", x1, x2, y1, y2,
                 width);
  };
  for (int i = -widthExtra / 2; i < width - (widthExtra / 2); i++) {
    gridLine(i, i, -height / 2, i, height / 2);
  }
  for (int i = -height / 2; i < height / 2; i++) {
    gridLine(i, -widthExtra / 2, i, width - (widthExtra / 2), i);
  }
  addRaw(R"(</g>)");
  // Grid labels
  addRaw(R"(<g font-size="1" fill="black" stroke="none")"
         R"( text-anchor="middle">)");
  for (int i = -widthExtra / 2; i < width - (widthExtra / 2); i++) {
    if (i % 10 == 0) {
      addFormatted(R"(<text x="%d" y="%d" dy="-0.5">%d%s</text>)", i,
                   height / 2, i, i == 0 ? "cm" : "");
    }
  }
  for (int i = -height / 2; i < height / 2; i++) {
    if (i % 10 == 0) {
      addFormatted(R"(<text x="%d" y="%d" dx="1.5">%d%s</text>)",
                   -widthExtra / 2, -i, i, i == 0 ? "cm" : "");
    }
  }
  addRaw(R"(</g>)");
  // Nose/tail labels
  addRaw(R"(<g font-size="1.5" fill="black" stroke="none">)");
  addFormatted(R"(<text x="%d" y="%d">Nose</text>)", (-widthExtra / 2) + 1,
               height / 2 - 5);
  addFormatted(R"(<text x="%d" y="%d" text-anchor="end">Tail</text>)",
               width - (widthExtra / 2) - 1, height / 2 - 5);
  addRaw(R"(</g>)");
}

void OverviewWriter::endDrawing() {
  assert(m_outputStream.is_open());
  m_outputStream << "</svg>" << std::endl;
}

void OverviewWriter::addPath(const Path& path, std::string color, bool dashed) {
  assert(m_outputStream.is_open());
  const char* dash = dashed ? "stroke-dasharray=\"1,0.25\" " : "";
  const char* opacity =
      dashed ? "stroke-opacity=\"0.5\" "
               "onmouseover=\"this.style['stroke-opacity']=1.0;\" "
               "onmouseout=\"this.style['stroke-opacity']=0.5;\" "
             : "";
  addFormatted(
      "<polyline %s%sstroke=\"%s\" "
      "stroke-width=\"0.1\" fill=\"none\" "
      "points=\"",
      dash, opacity, color.c_str());
  for (auto const& p : path) {
    m_outputStream << p.X.dbl() << "," << -p.Y.dbl() << " ";
  }
  m_outputStream << "\"></polyline>" << std::endl;
}

void OverviewWriter::addAnnotation(const DebugAnnotation& annotation) {
  assert(m_outputStream.is_open());
  auto& desc = annotation.desc();
  const char* dash = desc.dashed() ? "stroke-dasharray=\"1,0.25\" " : "";
  const char* opacity =
      desc.dashed() ? "stroke-opacity=\"0.5\" "
                      "onmouseover=\"this.style['stroke-opacity']=1.0;\" "
                      "onmouseout=\"this.style['stroke-opacity']=0.5;\" "
                    : "";
  addFormatted(
      "<g %s%sstroke=\"%s\" "
      "stroke-width=\"0.1\" fill=\"none\">",
      dash, opacity, desc.color().c_str());
  m_outputStream << annotation.svg().c_str() << std::endl;
  addRaw("</g>");
}

void OverviewWriter::addCode(std::function<void(std::ofstream&)> emitter) {
  assert(m_outputStream.is_open());
  addRaw("<pre>");
  emitter(m_outputStream);
  addRaw("</pre>");
}

}  // namespace MonkeyCAM
