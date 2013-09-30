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

// @TODO: load more parts
std::unique_ptr<ShapeEndPart> loadEndPart(boost::property_tree::ptree& config) {
  auto type = config.get<std::string>("type");
  auto endHandle = config.get<double>("end handle");
  auto transitionHandle = config.get<double>("transition handle");
  assert(type == "Basic Bezier");
  return std::unique_ptr<ShapeEndPart> {
    new BasicBezier { endHandle, transitionHandle } };
}

// @TODO: load more parts
std::unique_ptr<ShapeEdgePart> loadEdgePart(
  boost::property_tree::ptree& config)
{
  auto type = config.get<std::string>("type");
  assert(type == "Basic Arc");
  return std::unique_ptr<ShapeEdgePart> { new BasicArc };
}

std::unique_ptr<InsertPack> loadInserts(boost::property_tree::ptree& config) {
  auto countNose = config.get<int>("count nose");
  auto countTail = config.get<int>("count tail");
  auto offset = config.get<double>("offset", 4);
  auto hSpacing = config.get<double>("horizontal spacing", 4);
  auto vSpacing = config.get<double>("vertical spacing", 4);
  return std::unique_ptr<InsertPack> {
    new SnowboardInsertPack { countNose, countTail, offset,
        hSpacing, vSpacing } };
}

BoardShape loadBoard(boost::property_tree::ptree& config) {
  auto name = config.get<std::string>("board.name");
  auto noseLength = config.get<double>("board.nose length");
  auto eeLength = config.get<double>("board.effective edge length");
  auto tailLength = config.get<double>("board.tail length");
  auto sidecutRadius = config.get<double>("board.sidecut radius");
  auto waistWidth = config.get<double>("board.waist width");
  auto taper = config.get<double>("board.taper");
  auto nose = loadEndPart(config.get_child("board.nose shape"));
  auto edge = loadEdgePart(config.get_child("board.edge shape"));
  auto tail = loadEndPart(config.get_child("board.tail shape"));

  auto refStance = config.get<double>("board.reference stance width");
  auto setback = config.get<double>("board.stance setback");
  auto nosePack = loadInserts(config.get_child("board.nose insert pack"));
  auto tailPack = loadInserts(config.get_child("board.tail insert pack"));

  auto spacerWidth = config.get<double>("board.nose and tail spacer width");

  BoardShape board { name, noseLength, eeLength, tailLength, sidecutRadius,
      waistWidth, taper, nose, edge, tail, refStance, setback,
      nosePack, tailPack, spacerWidth };
  return board;
}

BoardProfile::End loadProfileEnd(boost::property_tree::ptree& config) {
  auto taperStart = config.get<double>("taper start");
  auto pullStart = config.get<double>("start handle");
  auto pullEnd = config.get<double>("end handle");
  auto taperEnd = config.get<double>("taper end");
  return BoardProfile::End { taperStart, pullStart, pullEnd, taperEnd };
}

BoardProfile loadProfile(boost::property_tree::ptree& config,
                         BoardShape& shape) {
  auto noseThickness = config.get<double>("profile.nose thickness");
  auto centerThickness = config.get<double>("profile.center thickness");
  auto tailThickness = config.get<double>("profile.tail thickness");
  auto noseEnd = loadProfileEnd(config.get_child("profile.nose taper"));
  auto tailEnd = loadProfileEnd(config.get_child("profile.tail taper"));

  BoardProfile profile { noseThickness, centerThickness, tailThickness,
      noseEnd, tailEnd, shape.eeLength(), shape.overallLength(),
      shape.noseLength() };
  return profile;
}

} // namespace MonkeyCAM


// @TODO: temp ghetto output to view a path with three.js.
void emitPathJS(std::ofstream& os, std::string name,
                const MonkeyCAM::Path& path) {
  os << "function getPath" << name << "() {" << std::endl;
  os << "  var path = new THREE.Geometry();" << std::endl;
  for (const auto& p : path) {
    os << "  path.vertices.push(new THREE.Vector3(";
    os << p.X.dbl() - (168 / 2) << ", "
       << p.Y.dbl() << ", "
       << p.Z.dbl() << "));"
       << std::endl;
  }
  os << "  return path;" << std::endl;
  os << "}" << std::endl;
}

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
  if (outdir[outdir.size() - 1] != '/') {
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
  shape.generateTopCutout(machine).write(outdir);
  shape.generateNoseTailSpacerCutout(machine).write(outdir);

  string overviewSvgName = shape.name() + "-overview.svg";
  printf("%s\n", overviewSvgName.c_str());
  MonkeyCAM::SVGWriter overallSvg(outdir + overviewSvgName, 16, 7);
  overallSvg.addPath(shape.buildOverallPath());
  overallSvg.addPath(profile.path());
  overallSvg.addPath(shape.buildCorePath(machine));

  // @TODO: temp ghetto output.
  std::ofstream os("paths.js");
  emitPathJS(os, "Core", shape.buildCorePath(machine));
  emitPathJS(os, "Overall", shape.buildOverallPath());
  emitPathJS(os, "Profile", profile.path());
  os.close();

  printf("Done.\n");
  return 0;
}
