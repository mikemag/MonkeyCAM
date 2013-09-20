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
#include "basic-math.h"

namespace MonkeyCAM {

// -----------------------------------------------------------------------------
// 2x2 matrix

Matrix2x2::Matrix2x2(double a, double b,
                     double c, double d) {
  m_data[0][0] = a;
  m_data[0][1] = b;
  m_data[1][0] = c;
  m_data[1][1] = d;
}

double Matrix2x2::Determinant() {
  return (m_data[0][0] * m_data[1][1]) - // ad
    (m_data[0][1] * m_data[1][0]); // bc
}

// -----------------------------------------------------------------------------
// 3x3 matrix

Matrix3x3::Matrix3x3(double a, double b, double c,
                     double d, double e, double f,
                     double g, double h, double i) {
  m_data[0][0] = a;
  m_data[0][1] = b;
  m_data[0][2] = c;
  m_data[1][0] = d;
  m_data[1][1] = e;
  m_data[1][2] = f;
  m_data[2][0] = g;
  m_data[2][1] = h;
  m_data[2][2] = i;
}

double Matrix3x3::Determinant() {
  return
    (m_data[0][0] * m_data[1][1] * m_data[2][2]) - // aei
    (m_data[0][0] * m_data[1][2] * m_data[2][1]) + // afh
    (m_data[1][0] * m_data[2][1] * m_data[0][2]) - // dhc
    (m_data[1][0] * m_data[0][1] * m_data[2][2]) + // dbi
    (m_data[2][0] * m_data[0][1] * m_data[1][2]) - // gbf
    (m_data[2][0] * m_data[1][1] * m_data[0][2]);  // gec
}

// -----------------------------------------------------------------------------
// A basic circle which can be constructed from three points

Circle::Circle(double p1X, double p1Y,
               double p2X, double p2Y,
               double p3X, double p3Y) {
  double p1x2y2 = pow(p1X, 2) + pow(p1Y, 2);
  double p2x2y2 = pow(p2X, 2) + pow(p2Y, 2);
  double p3x2y2 = pow(p3X, 2) + pow(p3Y, 2);
  Matrix3x3 Ma { p1X, p1Y, 1,
                 p2X, p2Y, 1,
                 p3X, p3Y, 1 };
  Matrix3x3 Md { p1x2y2, p1Y, 1,
                 p2x2y2, p2Y, 1,
                 p3x2y2, p3Y, 1 };
  Matrix3x3 Me { p1x2y2, p1X, 1,
                 p2x2y2, p2X, 1,
                 p3x2y2, p3X, 1 };
  Matrix3x3 Mf { p1x2y2, p1X, p1Y,
                 p2x2y2, p2X, p2Y,
                 p3x2y2, p3X, p3Y };
  double a = Ma.Determinant();
  double d = Md.Determinant() * -1;
  double e = Me.Determinant();
  double f = Mf.Determinant() * -1;
  centerX = -0.5 * d / a;
  centerY = -0.5 * e / a;
  radius = sqrt(((pow(d, 2) + pow(e, 2)) / (4 * pow(a, 2))) - (f / a));
}

} // namespace MonkeyCAM
