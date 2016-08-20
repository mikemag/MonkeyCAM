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

#include "shape-parts.h"

namespace MonkeyCAM {

const Path& BasicBezier::generate(Point endPoint, Point transitionPoint) {
  if (endPoint.X < transitionPoint.X) {
    m_path = BezierPath(endPoint,
                        Point(endPoint.X, (transitionPoint.Y - endPoint.Y) *
                              m_endHandle),
                        Point(transitionPoint.X -
                              ((transitionPoint.X - endPoint.X) *
                               m_transitionHandle), transitionPoint.Y),
                        transitionPoint);
  } else {
    m_path = BezierPath(transitionPoint,
                        Point(transitionPoint.X +
                              ((endPoint.X - transitionPoint.X) *
                               m_transitionHandle), transitionPoint.Y),
                        Point(endPoint.X, (transitionPoint.Y - endPoint.Y) *
                              m_endHandle),
                        endPoint);
  }
  return m_path;
}

const Path& FlatBezier::generate(Point endPoint, Point transitionPoint) {
  MCFixed flatEndY = - m_flatWidth / 2; 
  // Note: flatEndY is negative because the 'bottom' half of the board is drawn 
  //       first (i.e. in negative Y-space)
  BOOST_ASSERT_MSG(flatEndY >= transitionPoint.Y, 
                   "Error in generating \"Flat\" end: \"flat width\" is wider than nose/tail width");
  if (endPoint.X < transitionPoint.X) { 
    // Nose
    Path ep;
    ep.push_back(endPoint);
    m_path = BezierPath(Point(endPoint.X, flatEndY),
                        Point(endPoint.X, flatEndY + 
                              (transitionPoint.Y - flatEndY) * m_endHandle),
                        Point(transitionPoint.X -
                              ((transitionPoint.X - endPoint.X) *
                               m_transitionHandle), transitionPoint.Y),
                        transitionPoint);
    ep.push_back_path(m_path);
    m_path = ep;
  } else {
    // Tail
    m_path = BezierPath(transitionPoint,
                        Point(transitionPoint.X +
                              ((endPoint.X - transitionPoint.X) *
                               m_transitionHandle), transitionPoint.Y),
                        Point(endPoint.X, flatEndY + 
                              (transitionPoint.Y - flatEndY) * m_endHandle),
                        Point(endPoint.X, flatEndY));
    m_path.push_back(endPoint);
  }
  return m_path;
}
const Path& BasicArc::generate(Point nosePoint, Point waistPoint,
                               Point tailPoint) {
  m_path = ArcPath { nosePoint, waistPoint, tailPoint, ArcPath::Clockwise };
  return m_path;
}

} // namespace MonkeyCAM
