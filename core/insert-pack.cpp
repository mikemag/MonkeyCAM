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

#include <algorithm>
#include <vector>

#include "insert-pack.h"

namespace MonkeyCAM {

//------------------------------------------------------------------------------
// Base insert pack

void InsertPack::moveIntoPosition(Point center) {
  std::transform(m_insertsPath.begin(), m_insertsPath.end(),
                 m_insertsPath.begin(), [&](Point& p) { return p + center; });
}

//------------------------------------------------------------------------------
// Classic snowboard 4x4 insert pack

SnowboardInsertPack::SnowboardInsertPack(int countNose, int countTail,
                                         MCFixed offset,
                                         MCFixed hSpacing, MCFixed vSpacing)
    : m_offset(offset)
    , m_hSpacing(hSpacing)
    , m_vSpacing(vSpacing)
    , m_countNose(countNose)
    , m_countTail(countTail)
{
  addInsertGroup(0);
  for (int i = 1; i <= m_countNose; i++) {
    addInsertGroup(-m_offset * i);
  }
  for (int i = 1; i <= m_countTail; i++) {
    addInsertGroup(m_offset * i);
  }
  // We need to remove dups since the logic above can add dups
  // depending on the offset and spacing. Also other functions assume
  // the points are sorted.
  std::sort(m_insertsPath.begin(), m_insertsPath.end());
  auto newEnd = std::unique(m_insertsPath.begin(), m_insertsPath.end());
  m_insertsPath.resize(std::distance(m_insertsPath.begin(), newEnd));
}

void SnowboardInsertPack::addInsertGroup(MCFixed stanceX) {
  auto x = m_hSpacing / 2;
  auto y = m_vSpacing / 2;
  m_insertsPath.push_back(Point(stanceX - x, y));
  m_insertsPath.push_back(Point(stanceX - x, -y));
  m_insertsPath.push_back(Point(stanceX + x, y));
  m_insertsPath.push_back(Point(stanceX + x, -y));
}

//------------------------------------------------------------------------------
// Ski insert pack

SkiInsertPack::SkiInsertPack(const std::vector<double>& insertX, 
                             const std::vector<double>& insertY)
    //: m_insertX(insertX[])
    //, m_insertY(insertY[])
{
  
  m_insertsPath.push_back(Point(10, 0));

  std::sort(m_insertsPath.begin(), m_insertsPath.end());
  auto newEnd = std::unique(m_insertsPath.begin(), m_insertsPath.end());
  m_insertsPath.resize(std::distance(m_insertsPath.begin(), newEnd));
}

void SkiInsertPack::addInsertGroup() {
  m_insertsPath.push_back(Point(20, 5));
  m_insertsPath.push_back(Point(20, -5));
  m_insertsPath.push_back(Point(30, 5));
  m_insertsPath.push_back(Point(30, -5));
}

}
