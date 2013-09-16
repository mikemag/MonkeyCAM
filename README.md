# MonkeyCAM v4.0

MonkeyCAM v4.0 is an open source CAD/CAM program for designing skis and snowboards, and generating [G-code](http://en.wikipedia.org/wiki/G-code) programs to cut board parts with a [CNC machine](http://en.wikipedia.org/wiki/Numerical_control).

The [CAD](http://en.wikipedia.org/wiki/Computer-aided_design) portion of MonkeyCAM is quite limited, and is specifically designed to aid the design of skis and snowboards only. The [CAM](http://en.wikipedia.org/wiki/Computer-aided_manufacturing) portion is slightly more general, but again is specialized to ski and snowboard manufacture.

MonkeyCAM is published on Github at https://github.com/mikemag/MonkeyCAM

More information about ski and snowboard construction can be found at [Monkey Wiki](http://www.happymonkeysnowboards.com/MonkeyWiki) and [SkiBuilders.com](http://www.skibuilders.com/).

# License

MonkeyCAM is licensed under the Apache License, Version 2.0 except as otherwise notes. See [LICENSE](https://github.com/mikemag/MonkeyCAM/blob/master/LICENSE) for details.

The G-code programs **output** from MonkeyCAM are license-free.

# Features and Current Status

MonkeyCAM is currently a command-line program distributed in source-form only. It currently compiles with GCC 4.8.1 for Mac OSX. The program takes configuration files to describe the ski or snowboard shape via a small set of parameters and generates the follow G-code programs:

* Base cutout
* Core:
  * Guide holes to allow the core to be removed and flipped between programs
  * Alignment marks to transfer key design points to the bottom of the core
  * Edge relief to leave room for steel edges
  * Insert holes for standard snowboard inserts (T-nuts) in a variety of patterns
  * Top profile to impart the taper to the core **(not yet complete)**
  * Cutout, allowing for sidewall overhang and nose/tail spacers **(not yet complete)**
* Nose and tail spacers which match the core **(not yet complete)**
* Edge trenches to enable inlay of different edge wood and/or sidewall material along the effective edge of the core **(not yet complete)**

So far the G-code programs have been tested only on my CNC machine and control software. There is no UI, and all configuration and results are in files.

# Future Work

MonkeyCAM v4.0 is a work in-progress. It is an incomplete port of MonkeyCAM, developed by [Mike Magruder](https://github.com/mikemag) for [Happy Monkey Snowboards, Inc.](http://www.happymonkeysnowboards.com/) from 2003 to 2012. The valuable parts of the MonkeyCAM v3 backend (shape and G-code generation) will be ported to this version over the next many months. The old UI will be discarded. The plan includes, roughly in order:

* Complete the generation of the G-code programs marked **(not yet complete)** above.
* Target more G-code controllers with different formats and requirements.
* Provide distribution of binaries for Mac OSX and Windows 8.
* Port the MonkeyCAM design UI to a web-based UI built on HTML 5, and enable local execution. 
  * The UI will include easy controls for inputting the shape parameters, plus live visualization of the overall shape, profile, insert placement, etc. It also includes specifying the machining parameters. Right now I'm thinking about a combination of React and three.js.
* Port the MonkeyCAM G-code validation UI, built on the same framework as the design UI. 
  * This includes import of the G-code programs output from MonkeyCAM, as well as pretty much any G-code program, visualization of the tool path, overlay of multiple programs together for validation, and stepping through G-code programs to better understand how the cutter will move.
* Host MonkeyCAM on servers in something like AWS or Google Apps; serve the UI while running the engine on the server, provide local storage for board configurations.

Why not just open source MonkeyCAM v3? Honestly, the code for my personal version of MonkeyCAM has grown rather organically over almost 10 years and three major revisions. It is a huge mess of C#, WinForms, and Managed DirectX, and I'd really prefer no one but me ever sees that code again :)

# Dependencies

MonkeyCAM currently depends on the following packages:

* GCC 4.8.1. See http://gcc.gnu.org/ or install via something like Ports.
* CMake 2.8. See http://www.cmake.org/ or install via something like Ports.
* Boost 1.54.0. See http://www.boost.org/ or install via something like Ports.
* Clipper 5.1.6. See http://sourceforge.net/projects/polyclipping/. The CMake files assume this is installed in a sibling directory with the top-level MonkeyCAM directory, named clipper_ver5.1.6.

# Contributing

If you're a software developer who is into ski and snowboard construction or CAM and you're interested in helping out on MonkeyCAM then let's chat. Open up an issue with what you want to work on and we'll work out the details there, or fork the repo, make some changes and put up a pull request! 



