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

#ifndef incl_mcfixed_H_
#define incl_mcfixed_H_

#include <iostream>
#include <cstdint>
#include <boost/optional/optional.hpp>

namespace MonkeyCAM {

// Representation of distance used internally by MonkeyCAM to
// represent points. An integer holding a fixed-point number, with 4
// decimal places of precision.
//
// The units are centimeters.
//
struct MCFixed {
  MCFixed() : m_data(0) {}
  MCFixed(int i) : m_data(i * ScalingFactor) {}
  MCFixed(double d) : m_data(d * ScalingFactor) {}
  MCFixed(const MCFixed& mcf) : m_data(mcf.m_data) {}
  static MCFixed fromPreScaled(int64_t i) {
    MCFixed x;
    x.m_data = i;
    return x;
  }
  static MCFixed fromInches(double i) {
    return MCFixed(i * 2.54);
  }

  int64_t scaledInt() const { return m_data; }
  double dbl() const {
    return static_cast<double>(m_data) / static_cast<double>(ScalingFactor);
  }
  std::string str() const;
  std::string inchesStr() const;
  static std::string strWithSuffix(const boost::optional<MCFixed>& mcf) {
    if (mcf) {
      return (*mcf).str() + "cm";
    }
    return "None";
  }

  MCFixed operator+ (const MCFixed& mcf) const {
    return MCFixed(m_data + mcf.m_data);
  }
  MCFixed& operator+= (const MCFixed& mcf) {
    m_data += mcf.m_data;
    return *this;
  }
  MCFixed operator- (const MCFixed& mcf) const {
    return MCFixed(m_data - mcf.m_data);
  }
  MCFixed& operator-= (const MCFixed& mcf) {
    m_data -= mcf.m_data;
    return *this;
  }
  MCFixed operator- () const {
    return MCFixed(-m_data);
  }
  MCFixed operator* (const MCFixed& mcf) const {
    return MCFixed(m_data * mcf.m_data / ScalingFactor);
  }
  MCFixed& operator*= (const MCFixed& mcf) {
    m_data *= mcf.m_data;
    return *this;
  }
  MCFixed operator/ (const MCFixed& mcf) const {
    return MCFixed(m_data * ScalingFactor / mcf.m_data);
  }
  bool operator== (const MCFixed& mcf) const {
    return m_data == mcf.m_data;
  }
  bool operator!= (const MCFixed& mcf) const {
    return m_data != mcf.m_data;
  }
  bool operator< (const MCFixed& mcf) const {
    return m_data < mcf.m_data;
  }
  bool operator<= (const MCFixed& mcf) const {
    return m_data <= mcf.m_data;
  }
  bool operator> (const MCFixed& mcf) const {
    return m_data > mcf.m_data;
  }
  bool operator>= (const MCFixed& mcf) const {
    return m_data >= mcf.m_data;
  }

  friend std::ostream& operator << (std::ostream& outs, const MCFixed& mcf);

  constexpr static int64_t ScalingFactor = 10000;
  constexpr static int Precision = 4;

private:
  MCFixed(int64_t i) : m_data(i) {}
  int64_t m_data;
};

} // namespace MonkeyCAM

#endif // incl_mcfixed_H_
