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

#ifndef incl_paths_H_
#define incl_paths_H_

#include <vector>
#include "point.h"

namespace MonkeyCAM {

//------------------------------------------------------------------------------
// Base path. Supports an ordered set of points.

//typedef std::vector<Point> Path;
class Path : public std::vector<Point> {
 public:
  void push_back_path(const Path& path);
};

//------------------------------------------------------------------------------
// A path which is a portion of a circle.  Arcs are swept
// counter-clockwise from p1, thru p2, to p3.

class ArcPath : public Path {
 public:
  enum Direction {
    Clockwise,
    Counterclockwise
  };

  ArcPath(Point p1, Point p2, Point p3, Direction d = Counterclockwise);
};

//------------------------------------------------------------------------------
// A path which is a half ellipse.

class EllipsePath : public Path {
public:
  EllipsePath(Point center, MCFixed width, MCFixed height,
              double startAngle, double endAngle);
};

//------------------------------------------------------------------------------
// A path which is a bezier between two points, with two control points.

class BezierPath : public Path {
 public:
  BezierPath(Point p1, Point c1, Point c2, Point p2);
};

//------------------------------------------------------------------------------
// A path which mirrors another path across the Y axis, and reverses it.

class MirroredPath : public Path {
 public:
  MirroredPath(const Path& path);
};

//------------------------------------------------------------------------------
// A path which offsets another path using a given tool diameter. The
// source path has the height in Y, not Z, and Z is unused. Y is
// unchanged for all points in the output, only X is modified to
// ensure a cylindrical cutter traces the path properly.

class ToolOffsetPath : public Path {
 public:
  ToolOffsetPath(const Path& path, MCFixed toolDiameter);
};

//------------------------------------------------------------------------------
// A path which deforms the given path using the given profile path.

class ProfiledPath : public Path {
 public:
  ProfiledPath(const Path& path, const Path& profilePath);
};

//------------------------------------------------------------------------------
// A collection of help functions which operate on paths.

namespace PathUtils {

std::vector<Path> OffsetPath(const Path &path, MCFixed offset);
double Area(const Path& path);

} // namespace PathUtils

} // namespace MonkeyCAM

#endif // incl_paths_H_
