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

#ifndef incl_board_profile_H_
#define incl_board_profile_H_

#include "paths.h"

namespace MonkeyCAM {

//------------------------------------------------------------------------------

class BoardProfile {
 public:
  // Parameters to specify how the profile tapers from the center to
  // the end of a board. Each value is a percentage of half of the
  // board's effective edge, as measured from the center of the board.
  struct End {
    double taperStart;
    double pullStart; // Bezier control point for the start.
    double pullEnd; // Bezier control point for the end.
    double taperEnd;
  };

  BoardProfile(MCFixed noseThickness, MCFixed centerThickness,
               MCFixed tailThickness, End nose, End tail,
               MCFixed eeLength, MCFixed overallLength, MCFixed noseLength);
  const Path& path() const { return m_path; }

 private:
  MCFixed m_noseThickness;
  MCFixed m_centerThickness;
  MCFixed m_tailThickness;
  End m_nose;
  End m_tail;

  Path m_path;
};

} // namespace MonkeyCAM

#endif // incl_board_profile_H_
