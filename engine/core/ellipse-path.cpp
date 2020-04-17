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

#include "paths.h"

namespace MonkeyCAM {

// @todo: factor this into a util or math area
static double degreesToRadians(double degrees) {
  return degrees * 3.14159265 / 180.0;
}

EllipsePath::EllipsePath(Point center, MCFixed width, MCFixed height,
                         double startAngle, double endAngle) {
  startAngle = degreesToRadians(startAngle);
  endAngle = degreesToRadians(endAngle);
  int pointCount = 20;
  double angleDelta = (endAngle - startAngle) / pointCount;
  for (int i = 0; i < pointCount; i++) {
    double angle = startAngle + (angleDelta * i);
    emplace_back(Point(center.X + width * MCFixed(cos(angle)),
                       center.Y - height * MCFixed(sin(angle))));
  }
  emplace_back(Point(center.X + width * MCFixed(cos(endAngle)),
                     center.Y - height * MCFixed(sin(endAngle))));
}

}  // namespace MonkeyCAM
