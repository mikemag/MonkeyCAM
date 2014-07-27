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

#include <algorithm>
#include <cassert>
#include <boost/range/adaptor/reversed.hpp>
#include "../../clipper_ver5.1.6/cpp/clipper.hpp"
#include "paths.h"

namespace MonkeyCAM {

//------------------------------------------------------------------------------
// Path

void Path::push_back_path(const Path& path) {
  insert(end(), path.begin(), path.end());
}

//------------------------------------------------------------------------------
// MirroredPath

MirroredPath::MirroredPath(const Path& path) {
  for (int i = path.size() - 1; i >= 0; i--) {
    auto p = path[i];
    emplace_back(Point(p.X, -p.Y));
  }
}

//------------------------------------------------------------------------------
// ToolOffsetPath

// @TODO: this isn't robust at all. It works for profile paths with
// large enough flat zones, but is untested for anything
// else. Adjacent flat zones and valleys will cause loops.
ToolOffsetPath::ToolOffsetPath(const Path& path, MCFixed toolDiameter) {
  // NB: assumes the start and ends are flat, and the flat span is
  // longer than the diameter of the tool.
  assert(path.size() > 4);
  assert(path[0].Y == path[1].Y);
  assert(path[path.size() - 2].Y == path[path.size() - 1].Y);
  push_back(path.front());
  MCFixed prevOffset = 0;
  bool lastPointMoved = false;
  for (unsigned int i = 1; i < path.size() - 1; i++) {
    auto p1 = path[i - 1];
    auto p2 = path[i];
    // Which way does the line segment go?
    MCFixed offset;
    if (p1.Y > p2.Y) offset = toolDiameter / 2; // Decending
    else if (p1.Y < p2.Y) offset = -toolDiameter / 2; // Ascending
    else offset = 0; // Flat
    // If the start of the line segment was not moved, and we are
    // moving the end of the segment, then go ahead and move the start
    // of the segment.
    if (!lastPointMoved && (offset != 0)) back().X += offset;
    // If the start of the line segment was moved, and we're moving
    // the end of the segment in a different direction, then we need
    // to insert a new start point for the new line segment.
    if (lastPointMoved && (offset != 0) && (offset != prevOffset)) {
      push_back(p1 + Point(offset, 0));
    }
    // Offset the end point.
    push_back(p2 + Point(offset, 0));
    lastPointMoved = offset == 0 ? false : true;
    prevOffset = offset;
  }
  push_back(path.back());
}

//------------------------------------------------------------------------------
// ProfiledPath

// Return the first point in the profile array that the given line
// segment crosses. If the line segment crosses no points in the
// array, then the point returned is null. "Crossing" means actually
// crossing over a point. If one end point lies on a profile point,
// that doesn't count. The line segment is directional, with the start
// point being p1 and the end point being p2. If the line goes from
// left to right, then the profile points are considered from left to
// right. If the line goes from right to left, then the profile points
// are considered from right to left.
bool segmentCrossesProfilePoint(Point p1, Point p2, const Path& profilePath,
                                Point& crossingPoint) {
  if (p1.X < p2.X) { // Left to right
    for (auto& p : profilePath) {
      if ((p1.X < p.X) && (p.X < p2.X)) {
        crossingPoint = p;
        return true;
      }
    }
  } else {
    // Right to left. The rare special case of p1.X == p2.X falls in
    // here, too. That's okay, since we'll get the proper result of false
    // for that no matter how we do this.
    for (auto& p : boost::adaptors::reverse(profilePath)) {
      if ((p2.X < p.X) && (p.X < p1.X)) {
        crossingPoint = p;
        return true;
      }
    }
  }
  return false;
}

// Given a line segment p1->p2, and a point to break the segment at
// (bp), return a new point with X = bp.X, and Y computed properly. Z
// value is the same as the start point (p1).
Point breakLineSegmentAtPoint(Point p1, Point p2, Point bp) {
  // Simply intersect p1->p2 with a short vertical line at bp.X.
  return Point::Intersect(p1, p2, Point(bp.X, 0), Point(bp.X, 1));
}

// Given a point find the start and end points of the profile region
// the point lives in.
void getRegionBounds(const Path& profilePath, Point p,
                     Point& rStart, Point& rEnd) {
  for (unsigned int i = 0; i < profilePath.size() - 1; i++) {
    if ((profilePath[i].X <= p.X) && (p.X < profilePath[i + 1].X)) {
      rStart = profilePath[i];
      rEnd = profilePath[i + 1];
      return;
    }
  }
  rStart = *(profilePath.end() - 2);
  rEnd = *(profilePath.end() - 1);
  return;
}

ProfiledPath::ProfiledPath(const Path& path, const Path& profilePath)
{
  // First, form the new path from the original by breaking line
  // segments where they cross the profile points.
  for (unsigned int i = 0; i < path.size() - 1; i++) {
    auto p1 = path[i];
    auto p2 = path[i + 1];
    bool crossedPoint = false;
    do {
      push_back(p1); // p1 goes right into the new path.
      // Does p1->p2 cross a profile point?
      Point crossingPoint;
      crossedPoint = segmentCrossesProfilePoint(p1, p2, profilePath,
                                                crossingPoint);
      if (crossedPoint) {
        // Yup, so go ahead and break the line at the profile point.
        auto newPoint = breakLineSegmentAtPoint(p1, p2, crossingPoint);
        // newPoint.X had better be the same as the crossing point's
        // X. The only reason it will be different is due to rounding
        // errors, so we force the values to match here to avoid
        // infinite loops if it turns out that the rounded X value is
        // on the wrong side of the profile point.
        newPoint.X = crossingPoint.X;
        // We'll loop again, push the new point into the array, and
        // reconsider newPoint->p2 until we no longer cross a profile
        // point.
        p1 = newPoint;
      }
    } while (crossedPoint);
  }
  // The last point just goes right on the end.
  push_back(path.back());

  // Now, we have a path that has line segments that only live in one
  // profile region. So we simply set the Z value of every path point
  // based on it's location within a region. (Line segments no longer
  // matter.)
  for (auto& p : *this) {
    Point rStart;
    Point rEnd;
    getRegionBounds(profilePath, p, rStart, rEnd);
    // If the region is level, then just use the Z value from the
    // region start point.
    if (rStart.Y == rEnd.Y) {
      p.Z = rStart.Y;
    } else {
      // The region is sloped, so intersect the region line with a
      // vertical line at p.X.
      auto zp = breakLineSegmentAtPoint(rStart, rEnd, p);
      p.Z = zp.Y;
    }
  }
}

//------------------------------------------------------------------------------
// DebugPath

DebugPath::DebugPath(const std::string name, const std::string color,
                     const Path& path)
    : m_name(name)
    , m_color(color)
{
  push_back_path(path);
}


//------------------------------------------------------------------------------
// Utils

namespace PathUtils {

enum PathClosure {
  Open,
  Closed
};

ClipperLib::Polygon pathToPolygon(const Path& path, PathClosure c = Closed) {
  // Our paths are closed, and may have duplicated points in
  // them. Clean them up as we form the polygon.
  ClipperLib::Polygon poly;
  auto pathStart = path.cbegin();
  auto pathEnd = path.cend();
  Point lastPoint;
  assert((c == Open) || *(pathEnd - 1) == *pathStart);
  if (c == Closed) --pathEnd;
  for (auto p = pathStart; p != pathEnd; ++p) {
    if ((p == pathStart) || (lastPoint != *p)) {
      poly.push_back(ClipperLib::IntPoint(p->X.scaledInt(),
                                          p->Y.scaledInt()));
    }
    lastPoint = *p;
  }
  return poly;
}

Path polygonToPath(const ClipperLib::Polygon& poly) {
  Path path;
  for (const auto& p : poly) {
    path.emplace_back(Point(MCFixed::fromPreScaled(p.X),
                            MCFixed::fromPreScaled(p.Y)));
  }
  auto smallest = std::min_element(path.begin(), path.end());
  std::rotate(path.begin(), smallest, path.end());
  path.emplace_back(path[0]); // Poly is open, so close it now
  return path;
}

// Use Clipper to offset a path. The path is assumed to be
// counterclockwise on entry, and closed. The input path may have
// duplicate points.
//
// The output is a set of paths. They are counterclockwise, closed,
// and the start point is the minimum point (far left, lowest).
std::vector<Path> OffsetPath(const Path& path, MCFixed offset) {
  std::vector<Path> resultPaths;
  ClipperLib::Polygons origPolys;
  origPolys.emplace_back(pathToPolygon(path));
  assert(std::all_of(origPolys.cbegin(), origPolys.cend(),
                     ClipperLib::Orientation));
  ClipperLib::Polygons resultPolys;
  ClipperLib::OffsetPolygons(origPolys, resultPolys, offset.scaledInt(),
                             ClipperLib::jtRound, 0, false);
  for (const auto& poly : resultPolys) {
    resultPaths.emplace_back(polygonToPath(poly));
  }
  return resultPaths;
}

// Use Clipper to offset a set of open paths, i.e., lines. There is no
// assumption about the direction of the lines. The input lines may
// have duplicate points.
//
// The output is a set of paths. They are counterclockwise, closed,
// and the start point is the minimum point (far left, lowest).
std::vector<Path> OffsetLines(const std::vector<Path>& paths, MCFixed offset) {
  std::vector<Path> resultPaths;
  ClipperLib::Polygons origPolys;
  for (const auto& path : paths) {
    origPolys.emplace_back(pathToPolygon(path, Open));
  }
  ClipperLib::Polygons resultPolys;
  ClipperLib::OffsetPolyLines(origPolys, resultPolys, offset.scaledInt(),
                              ClipperLib::jtRound, ClipperLib::etButt, 0);
  for (const auto& poly : resultPolys) {
    resultPaths.emplace_back(polygonToPath(poly));
  }
  return resultPaths;
}


// Use Clipper to subtract one set of paths from another.
std::vector<Path> ClipPathsDifference(const std::vector<Path>& subjects,
                                      const std::vector<Path>& clips) {
  std::vector<Path> resultPaths;
  ClipperLib::Clipper c;
  ClipperLib::Polygons clip;
  ClipperLib::Polygons subject;
  ClipperLib::Polygons solution;
  for (auto& p : subjects) subject.emplace_back(pathToPolygon(p));
  for (auto& p : clips) clip.emplace_back(pathToPolygon(p));
  c.AddPolygons(subject, ClipperLib::ptSubject);
  c.AddPolygons(clip, ClipperLib::ptClip);
  c.Execute(ClipperLib::ctDifference, solution,
            ClipperLib::pftNonZero, ClipperLib::pftNonZero);
  for (const auto& poly : solution) {
    resultPaths.emplace_back(polygonToPath(poly));
  }
  return resultPaths;
}

// Assumes path is counterclockwise and closed.
double Area(const Path& path) {
  ClipperLib::Polygon poly = pathToPolygon(path);
  assert(ClipperLib::Orientation(poly));
  return ClipperLib::Area(poly) /
    (MCFixed::ScalingFactor * MCFixed::ScalingFactor);
}

void RemoveShortLines(Path& path, MCFixed minLength) {
  auto len2 = minLength.dbl() * minLength.dbl(); // Deal in squares below
  auto lineStart = path.begin();
  auto lineEnd = lineStart + 1;
  while (lineEnd != path.end()) {
    if (lineStart->distance2(*lineEnd) >= len2) {
      ++lineStart;
      *lineStart = std::move(*lineEnd);
    }
    ++lineEnd;
  }
  if (*lineStart != path.back()) {
    // The end didn't make it in... the line segment is too short. But
    // the start and end point are special; we want them to both stay
    // in the path. So force the end back in.
    *lineStart = std::move(path.back());
  }
  assert(lineStart < path.end());
  path.resize(std::distance(path.begin(), lineStart + 1));
}

// Create a simple lead-in path for the given path. We back up from
// the start of the path at least the given length and start from
// there, smoothly transitioning from the start height to the height
// of the first point on the path. If we hit the actual height of the
// path along the way, we take the rest of the path as-is. If the path
// is shorter than the lead-in length, then we simply spiral down the
// path, possibly multiple times.
Path SimpleLeadIn(const Path& path, MCFixed startHeight, MCFixed length) {
  Path leadIn;
  if (length == 0) return leadIn;
  assert(path.size() > 1);
  const double leadInLen = length.dbl();
  // Backup until we pass the lead-in length.
  int i = path.size() - 1;
  int last = i;
  double remainingLen = leadInLen;
  while (remainingLen > 0) {
    leadIn.push_back(path[last]);
    if (--i < 0) i = path.size() - 1;
    remainingLen -= path[i].distance(path[last]);
    last = i;
  }
  leadIn.push_back(path[last]);
  std::reverse(leadIn.begin(), leadIn.end());
  // Adjust heights until we hit the path.
  double heightDelta = (startHeight - path[0].Z).dbl();
  assert(heightDelta > 0);
  double usedLen = 0;
  double currentHeight = startHeight.dbl();
  for (unsigned int j = 0; j < leadIn.size() - 1; j++) {
    if (MCFixed(currentHeight) <= leadIn[j].Z) break;
    usedLen += leadIn[j].distance(leadIn[j + 1]);
    leadIn[j].Z = currentHeight;
    currentHeight = startHeight.dbl() - ((usedLen / leadInLen) * heightDelta);
  }
  return leadIn;
}

} // namespace PathUtils

} // namespace MonkeyCAM
