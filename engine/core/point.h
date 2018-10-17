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

#ifndef incl_Point_H_
#define incl_Point_H_

#include "../util/basic-math.h"
#include "mcfixed.h"

namespace MonkeyCAM {

class Point {
 public:
  Point() : X(0), Y(0), Z(0) {}
  Point(const Point& p) : X(p.X), Y(p.Y), Z(p.Z) {}
  Point(MCFixed x, MCFixed y) : X(x), Y(y), Z(0) {}
  Point(MCFixed x, MCFixed y, MCFixed z) : X(x), Y(y), Z(z) {}

  bool operator==(const Point& p) const {
    return X == p.X && Y == p.Y && Z == p.Z;
  }
  bool operator!=(const Point& p) const { return !(p == *this); }
  bool operator<(const Point& p) const {
    return X < p.X || (X == p.X && Y < p.Y) ||
           (X == p.X && Y == p.Y && Z < p.Z);
  }

  Point operator+(const Point& p) const {
    return Point(X + p.X, Y + p.Y, Z + p.Z);
  }
  Point operator+(const Vector2D& v) const {
    return Point(X + v.X, Y + v.Y, Z);
  }
  Point& operator+=(const Point& p) {
    *this = *this + p;
    return *this;
  }
  Point operator-(const Point& p) const {
    return Point(X - p.X, Y - p.Y, Z - p.Z);
  }
  Point operator-(const Vector2D& v) const {
    return Point(X - v.X, Y - v.Y, Z);
  }

  double distance(Point p) const;
  double distance2(Point p) const;
  Vector2D toVector2D() const { return Vector2D(X.dbl(), Y.dbl()); }

  static Point Intersect(Point p1, Point p2, Point p3, Point p4);

  static Point MinPoint;
  static Point MaxPoint;

  MCFixed X;
  MCFixed Y;
  MCFixed Z;
};

}  // namespace MonkeyCAM

#endif  // incl_Point_H_
