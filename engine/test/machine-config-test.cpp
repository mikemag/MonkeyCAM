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
#include <iostream>
#include <optional>
#include <string>

#include "activity-emitter.h"
#include "config.h"
#include "machine.h"

using MonkeyCAM::ActivityEmitter;
using MonkeyCAM::Config;
using MonkeyCAM::Machine;

namespace {

void fail(const char* message) {
  std::cerr << message << std::endl;
  std::exit(1);
}

}  // namespace

int main(int argc, char* argv[]) {
  if (argc != 2 && argc != 6) fail("unexpected arguments");
  ActivityEmitter::initialize(std::nullopt);
  Config config("Machine Configuration", argv[1]);
  Machine machine(config);

  // Invalid-input cases exit while constructing Machine and never reach here.
  if (argc == 2) fail("invalid configuration was accepted");

  if (machine.coreCutoutPasses() != std::stoi(argv[2]))
    fail("unexpected core cutout passes");
  if (machine.edgeTrenchPasses() != std::stoi(argv[3]))
    fail("unexpected edge trench passes");
  if (machine.topProfilePasses() != std::stoi(argv[4]))
    fail("unexpected top profile passes");

  const bool expectMaximum = std::string(argv[5]) == "true";
  const auto& maximum = machine.tool(1).maxCutDepth;
  if (maximum.has_value() != expectMaximum)
    fail("unexpected maximum depth presence");
  if (maximum && maximum->scaledInt() != 2000)
    fail("unexpected maximum depth value");
  return 0;
}
