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

#include <cstdlib>
#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <vector>

#include "cut-depth.h"

using MonkeyCAM::MCFixed;
using MonkeyCAM::planCutDepths;

namespace {

void fail(const char* message) {
  std::cerr << message << std::endl;
  std::exit(1);
}

void expectLevels(const std::vector<MCFixed>& actual,
                  const std::vector<int64_t>& expected) {
  if (actual.size() != expected.size()) fail("unexpected pass count");
  for (size_t i = 0; i < expected.size(); ++i) {
    if (actual[i].scaledInt() != expected[i]) fail("unexpected depth level");
  }
}

void expectInvalid(const std::function<void()>& work) {
  try {
    work();
  } catch (const std::invalid_argument&) {
    return;
  }
  fail("expected invalid_argument");
}

}  // namespace

int main() {
  const auto fixed = [](int64_t value) { return MCFixed::fromPreScaled(value); };

  // Neither passes nor maximum beyond the required default: one exact pass.
  expectLevels(planCutDepths(fixed(100), fixed(0), 1), {0});

  // Passes only: fixed-point remainder is spread over the last levels.
  expectLevels(planCutDepths(fixed(100), fixed(0), 3), {67, 34, 0});

  // A non-binding maximum preserves the configured pass count and schedule.
  expectLevels(planCutDepths(fixed(100), fixed(0), 3, fixed(34)),
               {67, 34, 0});

  // A binding maximum adds levels and no increment exceeds it.
  auto binding = planCutDepths(fixed(100), fixed(0), 2, fixed(24));
  expectLevels(binding, {80, 60, 40, 20, 0});
  int64_t previous = 100;
  for (const auto& level : binding) {
    if (previous - level.scaledInt() > 24) fail("maximum depth exceeded");
    previous = level.scaledInt();
  }

  // Exact division and a non-zero final depth.
  expectLevels(planCutDepths(fixed(110), fixed(10), 2, fixed(50)), {60, 10});

  expectInvalid([&] { planCutDepths(fixed(100), fixed(0), 0); });
  expectInvalid(
      [&] { planCutDepths(fixed(100), fixed(0), 1, fixed(0)); });
  expectInvalid([&] { planCutDepths(fixed(0), fixed(100), 1); });
  return 0;
}
