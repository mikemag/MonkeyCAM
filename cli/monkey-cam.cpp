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

#include <cmath>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <set>
#include <boost/optional/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sys/stat.h>

#include "machine.h"
#include "point.h"
#include "paths.h"
#include "shapes.h"
#include "gcode-writer.h"
#include "overview-writer.h"
#include "MonkeyCAMConfig.h"

using std::string;
using std::vector;
using std::set;

namespace MonkeyCAM {

// @TODO: load more parts
std::unique_ptr<ShapeEndPart> loadEndPart(boost::property_tree::ptree& config) {
  auto type = config.get<std::string>("type");
assert(type == "Basic Bezier" || type == "Flat");
  if(type == "Basic Bezier")
  {
    auto endHandle = config.get<double>("end handle");
    auto transitionHandle = config.get<double>("transition handle");

    
    return std::unique_ptr<ShapeEndPart> {
      new BasicBezier { endHandle, transitionHandle } };
  }
  else if(type == "Flat")
  {
    auto flatWidth = config.get<double>("flat width");
    auto endHandle = config.get<double>("end handle");
    auto transitionHandle = config.get<double>("transition handle");
    
    return std::unique_ptr<ShapeEndPart> {
      new FlatBezier {flatWidth, endHandle, transitionHandle } };
  }

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

std::unique_ptr<BoardShape> loadBoard(boost::property_tree::ptree& config) {
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
  auto spacerWidth = config.get<double>("board.nose and tail spacer width");

  boost::optional<MCFixed> refStance(
    config.get_optional<double>("board.reference stance width"));
  boost::optional<MCFixed> setback(
    config.get_optional<double>("board.stance setback"));
  auto npc = config.get_child_optional("board.nose insert pack");
  std::unique_ptr<InsertPack> nosePack;
  if (npc) {
    nosePack = loadInserts(npc.get());
  }
  std::unique_ptr<InsertPack> tailPack;
  auto tpc = config.get_child_optional("board.tail insert pack");
  if (tpc) {
    tailPack = loadInserts(tpc.get());
  }

  boost::optional<MCFixed> noseEdgeExt(
    config.get_optional<double>("board.nose edge extension"));
  boost::optional<MCFixed> tailEdgeExt(
    config.get_optional<double>("board.tail edge extension"));

  return std::unique_ptr<BoardShape> {
    new BoardShape { name, noseLength, eeLength, tailLength, sidecutRadius,
        waistWidth, taper, nose, edge, tail, refStance, setback,
        nosePack, tailPack, spacerWidth, noseEdgeExt, tailEdgeExt } };
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
      noseEnd, tailEnd, shape };
  return profile;
}

} // namespace MonkeyCAM


void usage(const char* program) {
  printf("\nUsage: %s --board brd.json --machine mach.json "
         "[--outdir <existing dir>]\n\n",
         program);
}

void generateOverview(MonkeyCAM::OverviewWriter& overview,
                      MonkeyCAM::BoardShape& shape,
                      MonkeyCAM::BoardProfile& profile) {
  overview.addRaw(R"(<div id="disclaimer">)"
                  R"(This data is provided on an "AS IS" BASIS WITHOUT )"
                  "WARRANTIES OR CONDITIONS OF ANY KIND, either express or "
                  "implied. You should inspect this very carefully and "
                  "recgonize that these overviews are not 100% accurate "
                  "representations of the generated G-Code programs. "
                  "YOU ARE RESPONSIBLE FOR ENSURING THOSE PROGRAMS WILL NOT "
                  "DAMAGE YOUR MACHINE OR MATERIALS.</div>");
  overview.addFormatted(R"(<h1 style="margin-bottom:0px;">%s</h1>)",
                        shape.name().c_str());
  time_t rawtime;
  time(&rawtime);
  struct tm* timeinfo = localtime(&rawtime);
  char timebuff[80];
  strftime(timebuff, sizeof(timebuff), "%Y-%m-%d %X %Z", timeinfo);
  overview.addFormatted(R"(<div id="generated">Generated by )"
                        R"(<a href="http://www.github.com/mikemag/MonkeyCAM">)"
                        R"(MonkeyCAM v%d.%d.%d</a> at %s</div>)",
                        MonkeyCAM_VERSION_MAJOR,
                        MonkeyCAM_VERSION_MINOR,
                        MonkeyCAM_VERSION_PATCH,
                        timebuff);
  overview.addFormatted(
    "<p>This is an overview of board %s, showing details of the overall shape, "
    "major dimensions, and diagrams of the primary G-Code paths against "
    "various reference shapes.</p>",
    shape.name().c_str());
  overview.addRaw(
    "<p>Every file generated by MonkeyCAM, including this one, is "
    "self-contained and does not rely on any other files generated or included "
    "within the MonkeyCAM distribution. Thus you can copy it wherever you'd "
    "like, send it to your friends, etc.</p>");
  overview.addRaw(
    "<p>This overview is designed for you to zoom into it using your favorite "
    "browser. All of the diagrams are built with vector graphics and will "
    "scale perfectly. Zoom in to get a good look at the details, how shapes "
    "fit together, and where they fall on the reference grid. In Chrome: "
    "&#8984+/&#8984- (or Ctrl+/Ctrl- for Windows).</p>");

  vector<const MonkeyCAM::DebugPathSet*> sets;
  for (auto pathSet : shape.debugPathSets()) {
    sets.push_back(pathSet);
    if (pathSet->header() == "Core shape") {
      sets.push_back(&profile.debugPathSet());
    }
  }

  overview.addRaw("<h4>Contents</h4><ul>");
  for (auto pathSet : sets) {
    overview.addFormatted("<li><a href=\"#%s\">%s</a></li>",
                          pathSet->headerLink().c_str(),
                          pathSet->header().c_str());
  }
  overview.addRaw("<li><a href=\"#Configuration\"</a>Configuration</li>");
  overview.addRaw("</ul>");

  int dw = std::ceil(shape.overallLength().dbl());
  int dh = std::ceil(shape.maxWidth().dbl());

  auto renderPathSet = [&](const MonkeyCAM::DebugPathSet& pathSet) {
    overview.addHeader(pathSet.header(), pathSet.headerLink());
    for (auto& desc : pathSet.descriptions()) {
      overview.addRaw(desc.c_str());
    }
    overview.addRaw("<dl>");
    set<string> seenDescs;
    auto descFormatter = [&](const MonkeyCAM::DebugAnnotationDesc& desc) {
      if (desc.name() != "" && seenDescs.find(desc.name()) == seenDescs.end()) {
        overview.addFormatted(
          R"(<dt id="pathname">%s (<span style="color:%s">%s</span>)</dt><dd>%s</dd>)",
          desc.name().c_str(),
          desc.color().c_str(),
          desc.color().c_str(),
          desc.desc().c_str());
        seenDescs.insert(desc.name());
      }
    };
    for (auto& path : pathSet.paths()) {
      descFormatter(path.desc());
    }
    for (auto& a : pathSet.annotations()) {
      descFormatter(a.desc());
    }
    overview.addRaw("</dl>");
    overview.startDrawing(dw, dh);
    for (auto& path : pathSet.paths()) {
      overview.addPath(path, path.desc().color(), path.desc().dashed());
    }
    for (auto& annotation : pathSet.annotations()) {
      overview.addAnnotation(annotation);
    }
    overview.endDrawing();
  };

  for (auto pathSet : sets) {
    renderPathSet(*pathSet);
  }
}

int main(int argc, char *argv[]) {
  printf("MonkeyCAM v%d.%d.%d, Copyright (C) 2013-2014 Michael M. Magruder\n",
         MonkeyCAM_VERSION_MAJOR,
         MonkeyCAM_VERSION_MINOR,
         MonkeyCAM_VERSION_PATCH);
  printf("This is free software; see the source for license information. There "
         "is\nNO warranty; not even for MERCHANTABILITY or FITNESS FOR A "
         "PARTICULAR PURPOSE.\n");
  printf("\n");

  string boardDef = "";
  string machineDef = "";
  string outdir = "";
  for (int i = 1; i < argc; ++i) {
    if ((string(argv[i]) == "--board") && (i + 1 < argc)) {
      boardDef = string(argv[++i]);
    } else if ((string(argv[i]) == "--machine") && (i + 1 < argc)) {
      machineDef = string(argv[++i]);
    } else if ((string(argv[i]) == "--outdir") && (i + 1 < argc)) {
      outdir = string(argv[++i]);
    }
  }
  if (boardDef == "") {
    printf("Missing required board definition file\n");
    usage(argv[0]);
    return 1;
  }
  if (machineDef == "") {
    printf("Missing required machine definition file\n");
    usage(argv[0]);
    return 1;
  }
  if (outdir == "") {
    outdir = ".";
  }
  struct stat stats;
  if ((stat(outdir.c_str(), &stats) != 0) || !S_ISDIR(stats.st_mode)) {
    printf("Output directory '%s' does not exist, please create it first.\n",
           outdir.c_str());
    return 1;
  }
#ifndef __MINGW32__
  if (outdir[outdir.size() - 1] != '/') {
    outdir += '/';
  }
#else
  if (outdir[outdir.size() - 1] != '\\') {
    outdir += '\\';
  }
#endif
  printf("Using board '%s', machine '%s'\n",
         boardDef.c_str(), machineDef.c_str());

  boost::property_tree::ptree machineConfig;
  read_json(machineDef, machineConfig);
  boost::property_tree::ptree boardConfig;
  read_json(boardDef, boardConfig);

  printf("Building board shapes...\n");
  const MonkeyCAM::Machine machine { machineConfig };
  auto shape = MonkeyCAM::loadBoard(boardConfig);
  auto profile = MonkeyCAM::loadProfile(boardConfig, *shape);

  printf("Generating G-code programs to '%s'...\n", outdir.c_str());
  // Force the overall and core shapes to generate first, so they are
  // first in the overview.
  shape->buildCorePath(machine);
  shape->generateTopProfile(machine, profile).write(outdir);
  shape->generateEdgeTrench(machine).write(outdir);
  shape->generateCoreEdgeGroove(machine).write(outdir);
  shape->generateNoseTailSpacerCutout(machine).write(outdir);
  shape->generateGuideHoles(machine).write(outdir);
  shape->generateCoreAlignmentMarks(machine).write(outdir);
  shape->generateBaseCutout(machine).write(outdir);
  shape->generateInsertHoles(machine).write(outdir);
  shape->generateTopCutout(machine).write(outdir);

  string overviewName = shape->name() + "-overview.html";
  printf("Generating HTML overview %s%s\n",
         outdir.c_str(), overviewName.c_str());
  MonkeyCAM::OverviewWriter overview(outdir + overviewName, shape->name());
  generateOverview(overview, *shape, profile);
  overview.addHeader("Configuration");
  overview.addRaw("<p>The configuration used to generate this overview is "
                  "echoed here to ensure it can always be regenerated later. "
                  "Note this is MonkeyCAM's interpretation of the files, "
                  "not the raw files.</p>");
  auto configLink = "https://github.com/mikemag/MonkeyCAM/blob/master/docs/Configuration_Guide.md";
  overview.addFormatted(
    R"(<p><a href="%s">Board configuration</a> file "%s":</p>)",
    configLink, boardDef.c_str());
  overview.addCode([&](std::ofstream& s) {
      write_json(s, boardConfig);
    });
  overview.addFormatted(
    R"(<p><a href="%s">Machine configuration</a> file "%s":</p>)",
    configLink, machineDef.c_str());
  overview.addCode([&](std::ofstream& s) {
      write_json(s, machineConfig);
    });

  printf("Done.\n");
  return 0;
}
