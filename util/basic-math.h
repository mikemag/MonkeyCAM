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

#ifndef incl_basic_math_H_
#define incl_basic_math_H_

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

} // namespace MonkeyCAM

#endif // incl_basic_math_H_
