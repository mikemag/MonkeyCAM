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

#include "paths.h"
#include "board-profile.h"

namespace MonkeyCAM {

BoardProfile::BoardProfile(MCFixed noseThickness, MCFixed centerThickness,
                           MCFixed tailThickness, End nose, End tail,
                           MCFixed eeLength, MCFixed overallLength,
                           MCFixed noseLength)
    : m_noseThickness(noseThickness)
    , m_centerThickness(centerThickness)
    , m_tailThickness(tailThickness)
    , m_nose(nose)
    , m_tail(tail)
{
  auto eeHalf = eeLength / 2;
  auto boardCenterX = eeHalf + noseLength;
  auto noseLandX = -eeHalf * m_nose.taperEnd + boardCenterX;
  auto cpNosePullStart = -eeHalf * m_nose.pullStart + boardCenterX;
  auto cpNosePullEnd = -eeHalf * m_nose.pullEnd + boardCenterX;
  auto noseDropX = -eeHalf * m_nose.taperStart + boardCenterX;
  auto tailLandX = eeHalf * m_tail.taperEnd + boardCenterX;
  auto tailDropX = eeHalf * m_tail.taperStart + boardCenterX;
  auto cpTailPullStart = eeHalf * m_tail.pullStart + boardCenterX;
  auto cpTailPullEnd = eeHalf * m_tail.pullEnd + boardCenterX;

  m_path.push_back(Point(0, m_noseThickness));
  BezierPath np {
    Point(noseLandX, m_noseThickness),
    Point(cpNosePullEnd, m_noseThickness),
    Point(cpNosePullStart, m_centerThickness),
    Point(noseDropX, m_centerThickness)
  };
  m_path.push_back_path(np);
  BezierPath tp {
    Point(tailDropX, m_centerThickness),
    Point(cpTailPullStart, m_centerThickness),
    Point(cpTailPullEnd, m_tailThickness),
    Point(tailLandX, m_tailThickness)
  };
  m_path.push_back_path(tp);
  m_path.push_back(Point(overallLength, m_tailThickness));
}

} // namespace MonkeyCAM
