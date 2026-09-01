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

#include "cut-depth.h"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <stdexcept>

namespace MonkeyCAM {

std::vector<MCFixed> planCutDepths(MCFixed startDepth, MCFixed finalDepth,
                                   int minimumPasses,
                                   std::optional<MCFixed> maxCutDepth) {
  if (minimumPasses <= 0) {
    throw std::invalid_argument("minimum passes must be greater than zero");
  }
  if (finalDepth > startDepth) {
    throw std::invalid_argument("final depth must not be above start depth");
  }
  if (maxCutDepth && *maxCutDepth <= 0) {
    throw std::invalid_argument("maximum cut depth must be greater than zero");
  }

  const int64_t totalDepth =
      startDepth.scaledInt() - finalDepth.scaledInt();
  int64_t passCount = minimumPasses;
  if (maxCutDepth && totalDepth > 0) {
    const int64_t maxDepth = maxCutDepth->scaledInt();
    const int64_t requiredPasses =
        (totalDepth / maxDepth) + ((totalDepth % maxDepth) == 0 ? 0 : 1);
    passCount = std::max(passCount, requiredPasses);
  }
  if (passCount > std::numeric_limits<int>::max()) {
    throw std::overflow_error("cut requires too many passes");
  }

  const int64_t baseIncrement = totalDepth / passCount;
  const int64_t remainder = totalDepth % passCount;
  int64_t cumulativeDepth = 0;
  std::vector<MCFixed> levels;
  levels.reserve(static_cast<size_t>(passCount));
  for (int64_t i = 0; i < passCount; ++i) {
    // Spread fixed-point remainder units over the last passes. This keeps the
    // levels as even as precision allows without accumulating rounding error.
    const int64_t increment =
        baseIncrement + (i >= passCount - remainder ? 1 : 0);
    cumulativeDepth += increment;
    levels.push_back(MCFixed::fromPreScaled(startDepth.scaledInt() -
                                             cumulativeDepth));
  }
  levels.back() = finalDepth;
  return levels;
}

}  // namespace MonkeyCAM
