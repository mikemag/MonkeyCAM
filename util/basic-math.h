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

#ifndef incl_basic_math_H_
#define incl_basic_math_H_

#include <cmath>

namespace MonkeyCAM {

static const double PI = 3.141592653589793;

struct Matrix2x2 {
  Matrix2x2(double a, double b,
            double c, double d);
  double Determinant();

 private:
  double m_data[2][2];
};

struct Matrix3x3 {
  Matrix3x3(double a, double b, double c,
            double d, double e, double f,
            double g, double h, double i);
  double Determinant();

 private:
  double m_data[3][3];
};

struct Circle {
  double centerX;
  double centerY;
  double radius;
  Circle(double p1X, double p1Y,
         double p2X, double p2Y,
         double p3X, double p3Y);
};

struct Vector2D {
  Vector2D(double x, double y) {
    X = x;
    Y = y;
    Normal = sqrt((X * X) + (Y * Y));
  }

  Vector2D(double p1X, double p1Y, double p2X, double p2Y) {
    X = p2X - p1X;
    Y = p2Y - p1Y;
    Normal = sqrt((X * X) + (Y * Y));
  }

  Vector2D toUnitVector() {
    return Vector2D(X / Normal, Y / Normal);
  }

  Vector2D toPerpVector() {
    return Vector2D(-Y, X);
  }

  Vector2D operator* (double d) {
    return Vector2D(X * d, Y * d);
  }
  Vector2D& operator*= (double d) {
    X *= d;
    Y *= d;
    return *this;
  }

  double X;
  double Y;
  double Normal;
};

} // namespace MonkeyCAM

#endif // incl_basic_math_H_
