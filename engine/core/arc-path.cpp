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

#include <cmath>
#include "basic-math.h"
#include "paths.h"

namespace MonkeyCAM {

ArcPath::ArcPath(Point p1, Point p2, Point p3, Direction d) {
  Circle c(p1.X.dbl(), p1.Y.dbl(), p2.X.dbl(), p2.Y.dbl(), p3.X.dbl(),
           p3.Y.dbl());
  // The interval of atan2 is (-PI/2, PI/2] with 0 at 3 oclock (i.e.,
  // for a circle of radius 1 centered at 0,0, atan yields 0 for point
  // 1,0), and positive values counterclockwise and negative values
  // clockwise.
  double startAngle = atan2(p1.Y.dbl() - c.centerY, p1.X.dbl() - c.centerX);
  double endAngle = atan2(p3.Y.dbl() - c.centerY, p3.X.dbl() - c.centerX);
  // Convert the interval to [0, 2PI)
  if (startAngle < 0) startAngle += 2 * PI;
  if (endAngle < 0) endAngle += 2 * PI;
  // Break the arc into a reasonable set of points and compute the
  // delta to sweep the points in the right direction. Compensate for
  // a sweep across angle zero, which requires that we flip the
  // direction.
  int pointCount = 100;  // @TODO: compute this based on some measure of error.
  double angleDelta = (endAngle - startAngle) / pointCount;
  if (startAngle > endAngle) angleDelta *= -1;
  if (d == Clockwise) angleDelta *= -1;
  emplace_back(p1);
  for (int i = 1; i < pointCount; i++) {
    double angle = startAngle + (angleDelta * i);
    emplace_back(Point(c.centerX + c.radius * cos(angle),
                       c.centerY + c.radius * sin(angle)));
  }
  emplace_back(p3);
}

}  // namespace MonkeyCAM
