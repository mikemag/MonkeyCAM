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

#include <cmath>
#include "paths.h"

namespace MonkeyCAM {

// Compute a single point along a 4 point bezier curve.
MCFixed bezier(double u, MCFixed x0, MCFixed x1, MCFixed x2, MCFixed x3) {
  double oneMinusU = 1.0 - u;
  double oneMinusU2 = pow(oneMinusU, 2.0);
  double oneMinusU3 = pow(oneMinusU, 3.0);
  double u2 = pow(u, 2.0);
  double u3 = pow(u, 3.0);
  return (x0.dbl() * oneMinusU3) +
    (x1.dbl() * 3.0 * u * oneMinusU2) +
    (x2.dbl() * 3.0 * u2 * oneMinusU) +
    (x3.dbl() * u3);
}

BezierPath::BezierPath(Point p1, Point c1, Point c2, Point p2) {
  double startU = 0.0;
  int pointCount = 50;
  double uDelta = 1.0 / pointCount;

  emplace_back(p1);
  for (int i = 1; i < pointCount; i++) {
    double u = startU + (uDelta * i);
    emplace_back(Point(bezier(u, p1.X, c1.X, c2.X, p2.X),
                       bezier(u, p1.Y, c1.Y, c2.Y, p2.Y)));
  }
  emplace_back(p2);
}

} // namespace MonkeyCAM
