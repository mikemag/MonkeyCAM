# MonkeyCAM v4.0

MonkeyCAM v4.0 is an open source CAD/CAM program for designing skis
and snowboards, and generating
[G-code](http://en.wikipedia.org/wiki/G-code) programs to cut board
parts with a [CNC
machine](http://en.wikipedia.org/wiki/Numerical_control).

The [CAD](http://en.wikipedia.org/wiki/Computer-aided_design) portion
of MonkeyCAM is quite limited, and is specifically designed to aid the
design of skis and snowboards only. The
[CAM](http://en.wikipedia.org/wiki/Computer-aided_manufacturing)
portion is slightly more general, but again is specialized to ski and
snowboard manufacture.

Here is an overview of what MonkeyCAM generates: https://rawgit.com/mikemag/MonkeyCAM/master/Sample-output/Sample-snowboard-overview.html

MonkeyCAM is published on Github at https://github.com/mikemag/MonkeyCAM

More information about ski and snowboard construction can be found at
[Monkey Wiki](http://www.happymonkeysnowboards.com/MonkeyWiki) and
[SkiBuilders.com](http://www.skibuilders.com/).

## Documentation

See the [MonkeyCAM v4.0 User’s Guide](
https://github.com/mikemag/MonkeyCAM/blob/master/docs/Users_Guide.md)
for an overview of how to use and configure MonkeyCAM.

## License

MonkeyCAM is licensed under the Apache License, Version 2.0 except as
otherwise noted. See
[LICENSE](https://github.com/mikemag/MonkeyCAM/blob/master/LICENSE)
for details.

The G-code programs **generated** by MonkeyCAM are license-free.

## Current Release

See the [Releases page](https://github.com/mikemag/MonkeyCAM/releases)
for the current release, which includes release notes and links to
download binaries.

## Features and Current Status

MonkeyCAM is currently a command-line program only. The program takes
configuration files to describe the ski or snowboard shape via a small
set of parameters and generates the following G-code programs plus a
HTML overview of the board:

* Base cutout
* Core:
  * Guide holes to allow the core to be removed and flipped between programs
  * Alignment marks to transfer key design points to the bottom of the core
  * Edge relief to leave room for steel edges
  * Insert holes for standard snowboard inserts (T-nuts) in a variety of patterns
  * Top profile to impart the taper to the core
  * Cutout, allowing for sidewall overhang and nose/tail spacers
* Nose and tail spacers which match the core
* Edge trenches to enable inlay of different edge wood and/or sidewall
material along the effective edge of the core

The G-code programs emitted at this time have been tested on my CNC
machine with a very old DeskCNC controller and work correctly. Each
program also loads in Mach 3 and simulates correctly. The programs not
involving holes load in ShopBot's controller and simulate correctly,
but the ones with holes do not work for ShopBot right now.

There is no interactive user interface. Configuration is done via two
text files, and all results are either G-Code programs, or the HTML
overview which can be easily used to refine the board shape.

## Future Work

MonkeyCAM v4.0 is a work in-progress. It is a port of MonkeyCAM v1-v3,
developed by [Mike Magruder](https://github.com/mikemag) for [Happy
Monkey Snowboards, Inc.](http://www.happymonkeysnowboards.com/) from
2003 to 2012. The valuable parts of the MonkeyCAM v3 backend (shape
and G-code generation) have been almost completely ported. The old UI
has been discarded.

My plan for work includes, roughly in order:

* Target more G-code controllers with different formats and requirements.
* More options for effective edge, nose, and tail. (MonkeyCAM v3, for
  instance, had 6 tail options.)
* Port the code to soften the transition between edge and
  nose/tail. (You can work around this with a sandblock and two min of
  your time for now.)
* Read the overall board shape from other CAD programs (DXF at least).
* Mold design and program to cut molds.
* Rouging passes for the core profile.
* Output the overall shape, insert locations, etc. to PDF/DXF for
  printing or use in other CAD programs.
* Host MonkeyCAM on servers in something like AWS or Google Apps;
  serve the UI while running the engine on the server, provide local
  storage for board configurations.
* Adapt for other kinds of 'boards': kite boards, skate boards, etc.

My plan does not currently include any further significant UI work. I
find the HTML overview generated as of v4.0.3 more than adequate to
iterate on design and understand what the parts will look like. I will
continue to refine the overview, of course. I hope others find it
equally easy to use.

If anyone is interested in building an interactive UI for MonkeyCAM
please contact me and I will be happy to help with logistics and
integrating the current engine with your UI. Some potential ideas to
consider:

* Create an interactive web-based UI built on HTML 5, and
  enable local execution.
  * The UI should include easy controls for inputting the shape
    parameters, plus live visualization of the overall shape, profile,
    insert placement, etc. It also should include specifying the machining
    parameters.
* Cretae a G-code validation UI, built on the same framework
  as the design UI.
  * This includes import of the G-code programs output from MonkeyCAM,
    as well as pretty much any G-code program, visualization of the
    tool path, overlay of multiple programs together for validation,
    and stepping through G-code programs to better understand how the
    cutter will move.

Why not just open source MonkeyCAM v3 and its UI? Honestly, the code
for older versions of MonkeyCAM had grown rather organically over
almost 10 years and three major revisions. It is a huge mess of C#,
WinForms, and Managed DirectX, and I'd really prefer no one but me
ever sees that code again. :)

## Dependencies

MonkeyCAM currently depends on the following packages:

* Mac build: Mac OSX 10.11.6 El Capitan with the system compiler, Apple LLVM version 7.3.0 (clang-703.0.31).
* Windows build: Windows 10.0, MinGW with GCC 4.9.3-1.
* CMake 3.6.1. See http://www.cmake.org/ or install via something like Ports.
* Boost 1.59.0. See http://www.boost.org/ or install via something like Ports.
* Clipper 5.1.6. See
  http://sourceforge.net/projects/polyclipping/. The CMake files
  assume this is installed in a sibling directory with the top-level
  MonkeyCAM directory, named clipper_ver5.1.6.

## Contributing

If you're a software developer who is into ski and snowboard
construction or CAM and you're interested in helping out on MonkeyCAM
then let's chat. Open up an issue with what you want to work on and
we'll work out the details there, or fork the repo, make some changes
and put up a pull request!

* [Design Overview](https://github.com/mikemag/MonkeyCAM/wiki/Design-Overview): details on the internal design of MonkeyCAM and how it creates shapes and G-Code programs.
* [Current issues and enhancements](https://github.com/mikemag/MonkeyCAM/issues)
