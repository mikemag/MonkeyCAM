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

#include "mcfixed.h"

#include <stdlib.h>
#include <stdio.h>

namespace MonkeyCAM {

std::ostream& operator << (std::ostream& outs, const MCFixed& mcf) {
  return outs << "MCF(" << mcf.m_data << ")";
}

std::string MCFixed::str() const {
  int64_t a = std::abs(m_data / ScalingFactor);
  int64_t b = std::abs(m_data % ScalingFactor);
  const char* minus = "";
  if ((m_data < 0) && ((a != 0) || (b != 0))) minus = "-";
  char buff[100];
  snprintf(buff, sizeof(buff), "%s%lld.%.*lld", minus, a, Precision, b);
  return std::string(buff);
}

std::string MCFixed::inchesStr() const {
  MCFixed x = *this / 2.54;
  int64_t a = std::abs(x.m_data / ScalingFactor);
  int64_t b = std::abs(x.m_data % ScalingFactor);
  const char* minus = "";
  if ((x.m_data < 0) && ((a != 0) || (b != 0))) minus = "-";
  char buff[100];
  snprintf(buff, sizeof(buff), "%s%lld.%.*lld", minus, a, Precision, b);
  return std::string(buff);
}

} // namespace MonkeyCAM
