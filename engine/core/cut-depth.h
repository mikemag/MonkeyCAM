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

#ifndef incl_cut_depth_H_
#define incl_cut_depth_H_

#include <optional>
#include <vector>

#include "mcfixed.h"

namespace MonkeyCAM {

// Return the absolute Z depths for a cut from startDepth to finalDepth. The
// configured pass count is a minimum; maxCutDepth may increase it. The final
// entry is always exactly finalDepth.
std::vector<MCFixed> planCutDepths(
    MCFixed startDepth, MCFixed finalDepth, int minimumPasses,
    std::optional<MCFixed> maxCutDepth = std::nullopt);

}  // namespace MonkeyCAM

#endif  // incl_cut_depth_H_
