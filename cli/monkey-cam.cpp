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

#include <iostream>
#include <cassert>
#include <vector>
#include <set>
#include <boost/optional/optional.hpp>
#include <sys/stat.h>

#include "machine.h"
#include "point.h"
#include "paths.h"
#include "shapes.h"
#include "gcode-writer.h"
#include "overview-writer.h"
#include "MonkeyCAMConfig.h"
#include "activity-emitter.h"
using ae = MonkeyCAM::ActivityEmitter;
#include "json.hpp"
using json = nlohmann::json;

using std::string;
using std::vector;
using std::set;

namespace MonkeyCAM {

template<class T>
boost::optional<T> jsonGetOptional(const json& obj, const std::string& key) {
  try {
    return boost::optional<T>(obj.at(key).get<T>());
  } catch (json::out_of_range& e) {
    return boost::optional<T>();
  }
}

// @TODO: load more parts
std::unique_ptr<ShapeEndPart> loadEndPart(const json& config) {
  std::string type = config.at("type");
  assert(type == "Basic Bezier" || type == "Flat");
  if (type == "Basic Bezier") {
    double endHandle = config.at("end handle");
    double transitionHandle = config.at("transition handle");

    return std::unique_ptr<ShapeEndPart> {
      new BasicBezier { endHandle, transitionHandle } };
  }
  else if (type == "Flat") {
    double flatWidth = config.at("flat width");
    double endHandle = config.at("end handle");
    double transitionHandle = config.at("transition handle");

    return std::unique_ptr<ShapeEndPart> {
      new FlatBezier {flatWidth, endHandle, transitionHandle } };
  }
  __builtin_unreachable();
}

// @TODO: load more parts
std::unique_ptr<ShapeEdgePart> loadEdgePart(const json& config)
{
  std::string type = config.at("type");
  assert(type == "Basic Arc");
  return std::unique_ptr<ShapeEdgePart> { new BasicArc };
}

std::unique_ptr<InsertPack> loadInserts(const json& config) {
  int countNose = config.at("count nose");
  int countTail = config.at("count tail");
  auto offset = config.value("offset", 4.0);
  auto hSpacing = config.value("horizontal spacing", 4.0);
  auto vSpacing = config.value("vertical spacing", 4.0);
  return std::unique_ptr<InsertPack> {
    new SnowboardInsertPack { countNose, countTail, offset,
        hSpacing, vSpacing } };
}

std::unique_ptr<InsertPack> loadSkiInsert(const json& config) {
  vector<double> insertX;
  vector<double> insertY;
  for (const auto& point : config) {
    insertX.push_back(point.at("x").get<double>());
    insertY.push_back(point.at("y").get<double>());
  }

  return std::unique_ptr<InsertPack> {
  new SkiInsertPack { insertX, insertY } };
}

std::unique_ptr<BoardShape> loadBoard(const json& config,
                                      const json& bindingConfig,
                                      double bindingDist) {
  auto board = config.at("board");
  std::string name = board.at("name");
  double noseLength = board.at("nose length");
  double eeLength = board.at("effective edge length");
  double tailLength = board.at("tail length");
  double sidecutRadius = board.at("sidecut radius");
  double waistWidth = board.at("waist width");
  double taper = board.at("taper");
  auto nose = loadEndPart(board.at("nose shape"));
  auto edge = loadEdgePart(board.at("edge shape"));
  auto tail = loadEndPart(board.at("tail shape"));
  double spacerWidth = board.at("nose and tail spacer width");

  boost::optional<MCFixed> setback(
    jsonGetOptional<double>(board, "stance setback"));

  // Inserts in board def file (retained for backwards compatibility)
  boost::optional<MCFixed> refStance(
    jsonGetOptional<double>(board, "reference stance width"));
  std::unique_ptr<InsertPack> nosePack;
  auto npc = jsonGetOptional<json>(board, "nose insert pack");
  if (npc) {
    nosePack = loadInserts(*npc);
  }
  std::unique_ptr<InsertPack> tailPack;
  auto tpc = jsonGetOptional<json>(board, "tail insert pack");
  if (tpc) {
    tailPack = loadInserts(*tpc);
  }

  // Inserts from binding def file
  auto binding = bindingConfig.at("binding");
  auto bndn = jsonGetOptional<json>(binding, "nose insert pack");
  if (bndn) {
    nosePack = loadInserts(*bndn);
  }
  auto bndtail = jsonGetOptional<json>(binding, "tail insert pack");
  if (bndtail) {
    tailPack = loadInserts(*bndtail);
  }
  std::unique_ptr<InsertPack> toeInserts;
  auto bndt = jsonGetOptional<json>(binding, "toe");
  if (bndt) {
    toeInserts = loadSkiInsert(*bndt);
  }
  std::unique_ptr<InsertPack> centerInserts;
  auto bndc = jsonGetOptional<json>(binding, "center");
  if (bndc) {
    centerInserts = loadSkiInsert(*bndc);
  }
  std::unique_ptr<InsertPack> heelInserts;
  auto bndh = jsonGetOptional<json>(binding, "heel");
  if (bndh) {
    heelInserts = loadSkiInsert(*bndh);
  }

  boost::optional<MCFixed> noseEdgeExt(
    jsonGetOptional<double>(board, "nose edge extension"));
  boost::optional<MCFixed> tailEdgeExt(
    jsonGetOptional<double>(board, "tail edge extension"));

  return std::unique_ptr<BoardShape> {
    new BoardShape { name, noseLength, eeLength, tailLength, sidecutRadius,
        waistWidth, taper, nose, edge, tail, refStance, setback, bindingDist,
        nosePack, tailPack, toeInserts, centerInserts, heelInserts, spacerWidth, noseEdgeExt, tailEdgeExt } };
}

BoardProfile::End loadProfileEnd(const json& config) {
  double taperStart = config.at("taper start");
  double pullStart = config.at("start handle");
  double pullEnd = config.at("end handle");
  double taperEnd = config.at("taper end");
  return BoardProfile::End { taperStart, pullStart, pullEnd, taperEnd };
}

BoardProfile loadProfile(const json& config, BoardShape& shape) {
  auto p = config.at("profile");
  double noseThickness = p.at("nose thickness");
  double centerThickness = p.at("center thickness");
  double tailThickness = p.at("tail thickness");
  auto noseEnd = loadProfileEnd(p.at("nose taper"));
  auto tailEnd = loadProfileEnd(p.at("tail taper"));

  BoardProfile profile { noseThickness, centerThickness, tailThickness,
      noseEnd, tailEnd, shape };
  return profile;
}

} // namespace MonkeyCAM


void usage(const char* program) {
  printf("\nUsage: %s --board brd.json --machine mach.json "
         "[--binding bnd.json] [--bindingdist <dist in cm>] "
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
                        R"(MonkeyCAM v%d.%d.%d %s@%s</a> at %s</div>)",
                        MonkeyCAM_VERSION_MAJOR,
                        MonkeyCAM_VERSION_MINOR,
                        MonkeyCAM_VERSION_PATCH,
                        MonkeyCAM_GIT_BRANCH,
                        MonkeyCAM_GIT_COMMIT_HASH,
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

void readJson(const string filename,
              json& config,
              const string configName) {
  std::ifstream f(filename);
  try {
    f >> config;
    f.close();
  } catch (json::parse_error& e) {
    string what = e.what();
    string msg = what.substr(what.find(':') + 2, std::string::npos);
    f.seekg(0);
    auto offset = e.byte;
    auto lineNumber = 1;
    auto remainingOffset = e.byte;
    char c;
    while (f.get(c) && --offset) {
      if (c == '\n') {
        lineNumber++;
        remainingOffset = offset;
      }
    }
    printf("Parse error in %s on line %d character %lu: %s\n",
           filename.c_str(), lineNumber, remainingOffset, msg.c_str());
    ae::emitter().write({
        {"parse error", {
            {"configName", configName},
            {"lineNumber", lineNumber},
            {"offset", remainingOffset},
            {"message", msg}
          }
        }
      });
    exit(1);
  }
};

int main(int argc, char *argv[]) {
  printf("MonkeyCAM v%d.%d.%d %s@%s, Copyright (C) 2013-2017 Michael M. Magruder\n",
         MonkeyCAM_VERSION_MAJOR,
         MonkeyCAM_VERSION_MINOR,
         MonkeyCAM_VERSION_PATCH,
         MonkeyCAM_GIT_BRANCH,
         MonkeyCAM_GIT_COMMIT_HASH);
  printf("This is free software; see the source for license information. There "
         "is\nNO warranty; not even for MERCHANTABILITY or FITNESS FOR A "
         "PARTICULAR PURPOSE.\n");
  printf("\n");

  boost::optional<int> jsonOutputFD;
  string boardDef = "";
  string machineDef = "";
  string bindingDef = "";
  double bindingDist = 0;
  string outdir = "";
  for (int i = 1; i < argc; ++i) {
    if ((string(argv[i]) == "--board") && (i + 1 < argc)) {
      boardDef = string(argv[++i]);
    } else if ((string(argv[i]) == "--machine") && (i + 1 < argc)) {
      machineDef = string(argv[++i]);
    } else if ((string(argv[i]) == "--binding") && (i + 1 < argc)) {
      bindingDef = string(argv[++i]);
    } else if ((string(argv[i]) == "--bindingdist") && (i + 1 < argc)) {
      bindingDist = std::atof(argv[++i]);
    } else if ((string(argv[i]) == "--outdir") && (i + 1 < argc)) {
      outdir = string(argv[++i]);
    } else if ((string(argv[i]) == "--jsonOutputFD") && (i + 1 < argc)) {
      jsonOutputFD = std::atoi(argv[++i]);
    }
  }

  ae::initialize(jsonOutputFD);

  if (jsonOutputFD) {
    ae::emitter().write({
        {"jsonOutputFD", *jsonOutputFD},
        {"progressMax", 14} // +1 over the true count to allow for saving ;)
      });
  }

  ae::emitter().write("version", "v%d.%d.%d",
                      MonkeyCAM_VERSION_MAJOR,
                      MonkeyCAM_VERSION_MINOR,
                      MonkeyCAM_VERSION_PATCH);
  ae::emitter().write({
      {"git", {
          {"branch", MonkeyCAM_GIT_BRANCH},
          {"commitHash", MonkeyCAM_GIT_COMMIT_HASH},
          {"commitDate", MonkeyCAM_GIT_COMMIT_DATE}
        }
      }
    });

  if (boardDef == "") {
    ae::emitter().fatal("Missing required board definition file");
    usage(argv[0]);
    return 1;
  }
  if (machineDef == "") {
    ae::emitter().fatal("Missing required machine definition file");
    usage(argv[0]);
    return 1;
  }
  if (outdir == "") {
    outdir = ".";
  }
  struct stat stats;
  if ((stat(outdir.c_str(), &stats) != 0) || !S_ISDIR(stats.st_mode)) {
    ae::emitter().fatal("Output directory '%s' does not exist, please create "
                        "it first.",
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
  printf("Using:\n  board: '%s'\n  machine: '%s'\n",
         boardDef.c_str(), machineDef.c_str());
  if (bindingDef != "") {
    printf("  binding: '%s'\n", bindingDef.c_str());
  }
  if (bindingDist > 0) {
    printf("  binding distance (board stance or ski boot length): %.2f cm\n",
           bindingDist);
  }

  json machineConfig;
  readJson(machineDef, machineConfig, "Machine Configuration");
  json boardConfig;
  readJson(boardDef, boardConfig, "Board Configuration");
  json bindingConfig;
  if (bindingDef != "") {
    readJson(bindingDef, bindingConfig, "Binding Configuration");
  }

  int progress = 1;
  ae::emitter().write("progress", "%d", progress++);

  // @TODO: produce better error messages about what nodes are missing or have
  // invalid values. Do this for both normal output and JSON output.
  printf("Building board shapes...\n");
  const MonkeyCAM::Machine machine { machineConfig };
  auto shape = MonkeyCAM::loadBoard(boardConfig, bindingConfig, bindingDist);
  auto profile = MonkeyCAM::loadProfile(boardConfig, *shape);
  ae::emitter().write("progress", "%d", progress++);

  printf("Generating G-code programs to '%s'...\n", outdir.c_str());
  // Force the overall and core shapes to generate first, so they are
  // first in the overview.
  shape->buildCorePath(machine);
  ae::emitter().write("progress", "%d", progress++);
  shape->generateTopProfile(machine, profile).write(outdir);
  ae::emitter().write("progress", "%d", progress++);
  shape->generateEdgeTrench(machine).write(outdir);
  ae::emitter().write("progress", "%d", progress++);
  shape->generateCoreEdgeGroove(machine).write(outdir);
  ae::emitter().write("progress", "%d", progress++);
  shape->generateNoseTailSpacerCutout(machine).write(outdir);
  ae::emitter().write("progress", "%d", progress++);
  shape->generateGuideHoles(machine).write(outdir);
  ae::emitter().write("progress", "%d", progress++);
  shape->generateCoreAlignmentMarks(machine).write(outdir);
  ae::emitter().write("progress", "%d", progress++);
  shape->generateBaseCutout(machine).write(outdir);
  ae::emitter().write("progress", "%d", progress++);
  shape->generateInsertHoles(machine).write(outdir);
  ae::emitter().write("progress", "%d", progress++);
  shape->generateTopCutout(machine).write(outdir);
  ae::emitter().write("progress", "%d", progress++);

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
  auto configLink = "https://github.com/mikemag/MonkeyCAM/blob/master/docs/"
    "Configuration_Guide.md";
  overview.addFormatted(
    R"(<p><a href="%s">Board configuration</a> file "%s":</p>)",
    configLink, boardDef.c_str());
  overview.addCode([&](std::ofstream& s) {
      s << std::setw(4) << boardConfig << std::endl;
    });
  overview.addFormatted(
    R"(<p><a href="%s">Machine configuration</a> file "%s":</p>)",
    configLink, machineDef.c_str());
  overview.addCode([&](std::ofstream& s) {
      s << std::setw(4) << machineConfig << std::endl;
    });
  if (bindingDef != "") {
    overview.addFormatted(
      R"(<p><a href="%s">Binding configuration</a> file "%s":</p>)",
      configLink, bindingDef.c_str());
    overview.addCode([&](std::ofstream& s) {
      s << std::setw(4) << bindingConfig << std::endl;
    });
  }
  ae::emitter().write("progress", "%d", progress++);

  printf("Done.\n");
  return 0;
}
