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

#include <climits>
#include <cmath>

#include "point.h"
#include "basic-math.h"

namespace MonkeyCAM {

Point Point::MinPoint {
  MCFixed::fromPreScaled(LLONG_MIN),
  MCFixed::fromPreScaled(LLONG_MIN),
  MCFixed::fromPreScaled(LLONG_MIN) };

Point Point::MaxPoint {
  MCFixed::fromPreScaled(LLONG_MAX),
  MCFixed::fromPreScaled(LLONG_MAX),
  MCFixed::fromPreScaled(LLONG_MAX) };

Point Point::Intersect(Point p1, Point p2, Point p3, Point p4) {
  Matrix2x2 xy12 { p1.X.dbl(), p1.Y.dbl(), p2.X.dbl(), p2.Y.dbl() };
  Matrix2x2 xy34 { p3.X.dbl(), p3.Y.dbl(), p4.X.dbl(), p4.Y.dbl() };
  auto x1mx2 = p1.X - p2.X;
  auto x3mx4 = p3.X - p4.X;
  auto y1my2 = p1.Y - p2.Y;
  auto y3my4 = p3.Y - p4.Y;
  Matrix2x2 divm { x1mx2.dbl(), y1my2.dbl(), x3mx4.dbl(), y3my4.dbl() };
  auto detdivm = divm.Determinant();
  auto detxy12 = xy12.Determinant();
  auto detxy34 = xy34.Determinant();
  Matrix2x2 ixtop { detxy12, x1mx2.dbl(), detxy34, x3mx4.dbl() };
  Matrix2x2 iytop { detxy12, y1my2.dbl(), detxy34, y3my4.dbl() };
  auto ix = ixtop.Determinant() / detdivm;
  auto iy = iytop.Determinant() / detdivm;
  return Point(ix, iy);
}

// Distance between two points.
double Point::distance(Point p) const {
  return sqrt(distance2(p));
}

// Computes the distance _squared_.
double Point::distance2(Point p) const {
  auto xd = (X - p.X).dbl();
  auto yd = (Y - p.Y).dbl();
  auto zd = (Z - p.Z).dbl();
  return (xd * xd) + (yd * yd) + (zd * zd);
}

}
