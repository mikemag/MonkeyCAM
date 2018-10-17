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

#ifndef incl_shape_parts_H_
#define incl_shape_parts_H_

#include "paths.h"

namespace MonkeyCAM {

//------------------------------------------------------------------------------
// End Parts: noses and tails

class ShapeEndPart {
 public:
  virtual const Path &generate(Point endPoint, Point transitionPoint) = 0;
  virtual ~ShapeEndPart() = default;

 protected:
  Path m_path;
};

class BasicBezier : public ShapeEndPart {
 public:
  BasicBezier(double endHandle, double transitionHandle)
      : m_endHandle(endHandle), m_transitionHandle(transitionHandle) {}

  const Path &generate(Point endPoint, Point transitionPoint);

 private:
  double m_endHandle;
  double m_transitionHandle;
};

class FlatBezier : public ShapeEndPart {
 public:
  FlatBezier(double flatWidth, double endHandle, double transitionHandle)
      : m_flatWidth(flatWidth),
        m_endHandle(endHandle),
        m_transitionHandle(transitionHandle) {}

  const Path &generate(Point endPoint, Point transitionPoint);

 private:
  double m_flatWidth;
  double m_endHandle;
  double m_transitionHandle;
};
//------------------------------------------------------------------------------
// Edge parts

class ShapeEdgePart {
 public:
  virtual const Path &generate(Point nosePoint, Point waistPoint,
                               Point tailPoint) = 0;
  virtual ~ShapeEdgePart() = default;

 protected:
  Path m_path;
};

class BasicArc : public ShapeEdgePart {
 public:
  BasicArc() {}
  const Path &generate(Point nosePoint, Point waistPoint, Point tailPoint);
};

}  // namespace MonkeyCAM

#endif  // incl_shape_parts_H_
