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

#ifndef incl_insert_pack_H_
#define incl_insert_pack_H_

#include "paths.h"

namespace MonkeyCAM {

//------------------------------------------------------------------------------

// An InsertPack represents one group of inserts on a board. It has a
// center, and provides a set of insert points around that center.
// When created, the center is at (0,0), but that is adjusted to it's
// true position when it is added to a BoardShape.
class InsertPack {
 public:
  const Path& insertsPath() const { return m_insertsPath; }
  void moveIntoPosition(Point center);
  const Point& minPoint() const { return m_insertsPath.front(); }
  const Point& maxPoint() const { return m_insertsPath.back(); }

 protected:
  InsertPack() {}
  Path m_insertsPath;
};

// Classic 4x4 snowboard inserts. Always generates a 4cm x 4cm group
// of 4 inserts around the center. Extra groups are added towarnds the
// nost and tail via countNose and countTail (zero is valid). Groups
// spacing is controlled via offset, and the horizontal and vertical
// spacing can be changed via hSpacing and vSpacing.
class SnowboardInsertPack : public InsertPack {
 public:
  SnowboardInsertPack(int countNose, int countTail,
                      MCFixed offset = 4,
                      MCFixed hSpacing = 4, MCFixed vSpacing = 4);

 private:
  void addInsertGroup(MCFixed stanceX);

  MCFixed m_offset;
  MCFixed m_hSpacing;
  MCFixed m_vSpacing;
  int m_countNose;
  int m_countTail;
};

} // namespace MonkeyCAM

#endif // incl_insert_pack_H_
