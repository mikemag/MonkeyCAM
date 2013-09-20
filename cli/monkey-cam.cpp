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

#include <cmath>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/program_options.hpp>

#include "machine.h"
#include "point.h"
#include "paths.h"
#include "shapes.h"
#include "gcode-writer.h"
#include "svg-writer.h"
#include "version.h"

using std::string;
using std::vector;

namespace MonkeyCAM {

// @TODO: need control points and kinds for nose, tail, and ee.
BoardShape loadBoard(boost::property_tree::ptree& config) {
  auto name = config.get<std::string>("board.name");
  auto noseLength = config.get<double>("board.nose length");
  auto eeLength = config.get<double>("board.effective edge length");
  auto tailLength = config.get<double>("board.tail length");
  auto sidecutRadius = config.get<double>("board.sidecut radius");
  auto waistWidth = config.get<double>("board.waist width");
  auto taper = config.get<double>("board.taper");
  auto refStance = config.get<double>("board.reference stance width");
  auto setback = config.get<double>("board.stance setback");

  auto countNose = config.get<int>("board.nose insert pack.count nose");
  auto countTail = config.get<int>("board.nose insert pack.count tail");
  auto offset = config.get<double>("board.nose insert pack.offset", 4);
  auto hSpacing =
    config.get<double>("board.nose insert pack.horizontal spacing", 4);
  auto vSpacing =
    config.get<double>("board.nose insert pack.vertical spacing", 4);
  SnowboardInsertPack nosePack { countNose, countTail, offset,
      hSpacing, vSpacing };

  countNose = config.get<int>("board.tail insert pack.count nose");
  countTail = config.get<int>("board.tail insert pack.count tail");
  offset = config.get<double>("board.tail insert pack.offset", 4);
  hSpacing = config.get<double>("board.tail insert pack.horizontal spacing", 4);
  vSpacing = config.get<double>("board.tail insert pack.vertical spacing", 4);
  SnowboardInsertPack tailPack { countNose, countTail, offset,
      hSpacing, vSpacing };

  BoardShape board { name, noseLength, eeLength, tailLength, sidecutRadius,
      waistWidth, taper, refStance, setback, nosePack, tailPack };
  return board;
}

BoardProfile loadProfile(boost::property_tree::ptree& config,
                         BoardShape& shape) {
  auto noseThickness = config.get<double>("profile.nose thickness");
  auto centerThickness = config.get<double>("profile.center thickness");
  auto tailThickness = config.get<double>("profile.tail thickness");

  auto taperStart = config.get<double>("profile.nose taper.taper start");
  auto pullStart = config.get<double>("profile.nose taper.start handle");
  auto pullEnd = config.get<double>("profile.nose taper.end handle");
  auto taperEnd = config.get<double>("profile.nose taper.taper end");
  BoardProfile::End noseEnd { taperStart, pullStart, pullEnd, taperEnd };

  taperStart = config.get<double>("profile.tail taper.taper start");
  pullStart = config.get<double>("profile.tail taper.start handle");
  pullEnd = config.get<double>("profile.tail taper.end handle");
  taperEnd = config.get<double>("profile.tail taper.taper end");
  BoardProfile::End tailEnd { taperStart, pullStart, pullEnd, taperEnd };

  BoardProfile profile { noseThickness, centerThickness, tailThickness,
      noseEnd, tailEnd, shape.eeLength(), shape.overallLength(),
      shape.noseLength() };
  return profile;
}

} // namespace MonkeyCAM


void usage(const char* program) {
  printf("\nUsage: %s --board brd.json --machine mach.json --outdir <dir>\n\n",
         program);
}

int main(int argc, char *argv[]) {
  printf("MonkeyCAM v%d.%d, Copyright (C) 2013 Michael M. Magruder\n",
         MC_VER_MAJOR, MC_VER_MINOR);
  printf("This is free software; see the source for license information. There "
         "is\nNO warranty; not even for MERCHANTABILITY or FITNESS FOR A "
         "PARTICULAR PURPOSE.\n");
  printf("\n");

  namespace po = boost::program_options;
  po::options_description desc("Options");
  desc.add_options()
    ("help,?", "Help")
    ("board", po::value<string>(), "Board defintion file (JSON)")
    ("machine", po::value<string>(), "Machine definition file (JSON)")
    ("outdir", po::value<string>(), "Output directory");
  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  } catch (boost::program_options::error& e) {
    printf("%s\n", e.what());
    return 1;
  }
  if (vm.count("help") || vm.empty()) {
    usage(argv[0]);
    std::cout << desc << "\n";
    return 1;
  }
  if (!vm.count("board")) {
    printf("Missing required board definition file\n");
    usage(argv[0]);
    return 1;
  }
  if (!vm.count("machine")) {
    printf("Missing required machine definition file\n");
    usage(argv[0]);
    return 1;
  }
  if (!vm.count("outdir")) {
    printf("Missing required output directory\n");
    usage(argv[0]);
    return 1;
  }
  auto boardDef = vm["board"].as<string>();
  auto machineDef = vm["machine"].as<string>();
  auto outdir = vm["outdir"].as<string>();
  if (outdir[outdir.size()] != '/') {
    outdir += '/';
  }
  printf("Using board '%s', machine '%s'\n",
         boardDef.c_str(), machineDef.c_str());

  boost::property_tree::ptree machineConfig;
  read_json(machineDef, machineConfig);
  boost::property_tree::ptree boardConfig;
  read_json(boardDef, boardConfig);

  printf("Building board shapes...\n");
  MonkeyCAM::Machine machine { machineConfig };
  auto shape = MonkeyCAM::loadBoard(boardConfig);
  auto profile = MonkeyCAM::loadProfile(boardConfig, shape);

  printf("Generating G-code programs to '%s'...\n", outdir.c_str());
  shape.generateBaseCutout(machine).write(outdir);
  shape.generateCoreAlignmentMarks(machine).write(outdir);
  shape.generateGuideHoles(machine).write(outdir);
  shape.generateCoreEdgeGroove(machine).write(outdir);
  shape.generateInsertHoles(machine).write(outdir);
  shape.generateTopProfile(machine, profile).write(outdir);

  string overviewSvgName = shape.name() + "-overview.svg";
  printf("%s\n", overviewSvgName.c_str());
  MonkeyCAM::SVGWriter overallSvg(outdir + overviewSvgName, 16, 7);
  overallSvg.addPath(shape.buildOverallPath());
  overallSvg.addPath(profile.path());

  printf("Done.\n");
  return 0;
}
