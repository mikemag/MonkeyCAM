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

#ifndef incl_gcode_writer_H_
#define incl_gcode_writer_H_

#include <string>
#include <vector>
#include <iostream>

#include "machine.h"
#include "paths.h"

namespace MonkeyCAM {

//------------------------------------------------------------------------------
// GCodeWriter
//
// Turn a single path into G-Code to drive a CNC machine. 3D paths are
// emitted without adjustment to Z. 2D paths are emitted following a
// variety of adjustments to Z to cut to various depths or thru the
// material.
//
// G54 is the WCO for the part.
// G55 is the WCO for the machine, to home after cutting the part.

class GCodeWriter {
 public:
  enum ZeroZ { MaterialTop, TableTop };
  enum XYOrigin { LowerLeft, YIsPartCenter };

  // Feed speeds are in IPM, suitable for a Fxx.
  GCodeWriter(std::string filename, Tool const& tool, ZeroZ zeroHeight,
              XYOrigin origin, int rapidSpeed, int normalSpeed,
              MCFixed rapidHeight);

  void spindleOn();
  void spindleOff();
  void rapidToPoint(Point p);
  void feedToPoint(Point p, int feedRate = -1); // -1 uses default speed
  void comment(std::string s = "", int indent = 0);
  void commentF(const char* fmt, ...);
  void headerComment(std::string s = "", int indent = 0);
  void headerCommentF(const char* fmt, ...);
  void line(std::string s = "");

  void emitPath(Path& path);
  void emitPath(Path& path, MCFixed depth);
  void emitSpiralPath(Path& path, MCFixed startDepth, int numberOfLevels);

  void emitClockwiseArc(Point dest, MCFixed radius);
  void emitIncrementalCircle(MCFixed diameter);
  void emitIncrementalHole(MCFixed diameter, MCFixed depth,
                           MCFixed heightAboveMaterial, int steps);

  void setIncremental();
  void setAbsolute();

  // @TODO: add more path emitters: with tabs, etc.
  // @TODO: add lead-in/outs

  void close();
  void write(std::string directory) const;
  void writeJS(std::string name, std::ofstream& stream) const;

  const Tool& tool() const { return m_tool; }
  const Point& currentPosition() const { return m_currentPosition; }

 private:
  typedef std::vector<std::string> GCodeLines;

  void commentWrapped(GCodeLines& lines, std::string s, int indent = 0);
  void comment(GCodeLines& lines, const char* fmt);
  void comment(GCodeLines& lines, const char* fmt, va_list args);
  void lineF(const char* fmt, ...);
  void headerBlock();
  void startBlock();
  void endBlock();
  void updateCurrentPosition(Point p);

  std::string m_filename;
  Tool m_tool;

  int m_rapidSpeed;
  int m_defaultSpeed;

  MCFixed m_rapidHeight;

  ZeroZ m_zeroHeight;
  XYOrigin m_xyOrigin;

  bool m_incremental;
  int m_currentSpeed;
  Point m_currentPosition;
  bool m_firstMovement;
  Point m_lowerBoundingBox;
  Point m_upperBoundingBox;

  GCodeLines m_headerComments;
  GCodeLines m_lines;
};

} // namespace MonkeyCAM

#endif // incl_gcode_writer_H_
