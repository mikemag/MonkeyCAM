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

#include <cmath>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/program_options.hpp>

#include "machine.h"
#include "point.h"
#include "paths.h"
#include "shapes.h"
#include "gcode-writer.h"
#include "svg-writer.h"
#include "version.h"

using std::string;
using std::vector;

namespace MonkeyCAM {

// ***************************************************************
// * These are snippets that should be tests, but aren't yet. No *
// * guarantee they even build (this file isn't built.)          *
// ***************************************************************

void dumpPath(const Path& path) {
  int count = 0;
  fprintf(stderr, "---- path dump ----\n");
  for (const auto& p : path) {
    fprintf(stderr, "#%d (%lld, %lld)\n", count++,
            p.X.scaledInt(), p.Y.scaledInt());
  }
}

void misc(Machine& machine) {
  Point p1(4.2, 4.2, 4.2);
  fprintf(stderr, "MonkeyCAM stub: %lld\n", p1.X.scaledInt());

  EllipsePath ep { Point(5, 5), 2, 3, 0, 180 };

  ArcPath ap1 { Point(1, 1), Point(3, 0.8), Point(6, 1) };
  ArcPath ap2 { Point(6, 1), Point(3, 1.3), Point(1, 1) };
  ArcPath ap3 { Point(1, -1), Point(2, 0), Point(1, 1) };
  ArcPath ap4 { Point(1, 1), Point(0, 0), Point(1, -1) };
  ArcPath ap5 { Point(2, 1), Point(1, 2), Point(0, 1) };
  ArcPath ap6 { Point(0, 1), Point(1, 0), Point(2, 1) };
  ArcPath ap7 { Point(1, 2), Point(3, 2.5), Point(6, 2), ArcPath::Clockwise };
  ArcPath ap8 { Point(6, 2), Point(3, 1.5), Point(1, 2), ArcPath::Clockwise };
  Path cp;
  cp.push_back_path(ap1);
  cp.push_back_path(ap2);
  cp.push_back_path(ap3);
  cp.push_back_path(ap4);
  cp.push_back_path(ap5);
  cp.push_back_path(ap6);
  cp.push_back_path(ap7);
  cp.push_back_path(ap8);
  //dumpPath(cp);
  BezierPath bp { Point(1, 1), Point(1.5, 2), Point(2, 3), Point(3, 2) };
  //dumpPath(bp);
  SVGWriter svg("tmp.svg");
  svg.addPath(ap1);
  svg.addPath(ap2);
  svg.addPath(ap3);
  svg.addPath(ap4);
  svg.addPath(ap5);
  svg.addPath(ap6);
  svg.addPath(ap7);
  svg.addPath(ap8);
  svg.addPath(bp, "#ff0000");
  svg.addPath(ep);

  boost::property_tree::ptree boardConfig;
  read_json("b1.json", boardConfig);
  auto shape = loadBoard(boardConfig);
  auto profile = loadProfile(boardConfig, shape);

  auto overallPath = shape.buildOverallPath();
  auto basePath = shape.buildBasePath();

  SVGWriter svgBoard("shape-overview.svg", 16, 7);
  svgBoard.addPath(overallPath);
  svgBoard.addPath(basePath);
  svgBoard.addPath(profile.path());

  SVGWriter osvg("top-profile-overview.svg", 16, 7);
  osvg.addPath(overallPath, "#0000ff");
  std::vector<Path> resultPaths;
  int i = 0;
  do {
    resultPaths = PathUtils::OffsetPath(overallPath, -1.5 * 2.54 / 2 * ++i);
    for (auto const& p : resultPaths) osvg.addPath(p, "#00ff00");
  } while (!resultPaths.empty());


  auto tmpProgram = shape.generateTopProfile(machine, profile);

#if 0
  for (const auto& l : tmpProgram) {
    fprintf(stderr, "%s\n", l.c_str());
  }
#endif
  //dumpPath(top1);
}

} // namespace MonkeyCAM
