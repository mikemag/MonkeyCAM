# MonkeyCAM v4

MonkeyCAM v4 is an open source CAD/CAM program for designing skis
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

The easiest way to use MonkeyCAM is at [MonkeyCAM.org](https://www.monkeycam.org). See the [tour](https://www.monkeycam.org/tour) for an overview of the site.

MonkeyCAM creates nine G-Code programs to cut all parts of a ski or snowboard on your CNC machine. For a real example of the results, see https://monkeycam.org/results/5744863563743232

MonkeyCAM is published on Github at https://github.com/mikemag/MonkeyCAM

More information about ski and snowboard construction can be found at
[Monkey Wiki](http://www.happymonkeysnowboards.com/MonkeyWiki) and
[SkiBuilders.com](http://www.skibuilders.com/).

## Donate

You can help offset the server costs for running [MonkeyCAM.org](https://www.monkeycam.org) with a small donation. 

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/H2H5OG3E0)


## Documentation

See the [MonkeyCAM v4 User’s Guide](https://github.com/mikemag/MonkeyCAM/blob/master/engine/docs/Users_Guide.md)
for an overview of how to use and configure MonkeyCAM.

## License

MonkeyCAM is licensed under the Apache License, Version 2.0 except as
otherwise noted. See
[LICENSE](https://github.com/mikemag/MonkeyCAM/blob/master/LICENSE)
for details.

The G-code programs **generated** by MonkeyCAM are license-free.

## Current Release

The current version of MonkeyCAM is always live at [MonkeyCAM.org](https://www.monkeycam.org), where you can use it without having to download it.

See the [Releases page](https://github.com/mikemag/MonkeyCAM/releases)
for the last engine release notes. Releases no longer contain binaries.

## Features and Current Status

The MonkeyCAM engine is currently a command-line program only, which is made available with a very basic UI at [MonkeyCAM.org](https://www.monkeycam.org). The program takes
configuration files to describe the ski or snowboard shape via a small
set of parameters and generates the following G-code programs plus a
HTML overview of the board:

- Base cutout
- Core:
  - Guide holes to allow the core to be removed and flipped between programs
  - Alignment marks to transfer key design points to the bottom of the core
  - Edge relief to leave room for steel edges
  - Insert holes for standard snowboard inserts (T-nuts) in a variety of patterns
  - Top profile to impart the taper to the core
  - Cutout, allowing for sidewall overhang and nose/tail spacers
- Nose and tail spacers which match the core
- Edge trenches to enable inlay of different edge wood and/or sidewall
  material along the effective edge of the core

The G-code programs emitted at this time have been tested on my CNC
machine with a very old DeskCNC controller and work correctly. Each
program also loads in Mach 3 and simulates correctly. The programs not
involving holes load in ShopBot's controller and simulate correctly,
but the ones with holes do not work for ShopBot right now.

There is no interactive user interface. Configuration is done via three
text files, and all results are either G-Code programs, or the HTML
overview which can be easily used to refine the board shape.

## History

MonkeyCAM was started in 2003 by [Mike Magruder](https://github.com/mikemag) for [Happy
Monkey Snowboards, Inc.](http://www.happymonkeysnowboards.com/). Older, private versions were built from
2003 to 2012. The open source version here was released in 2013. 
It has been available on the web at [MonkeyCAM.org](https://www.monkeycam.org) since 2017.

## Future Work

There is no feature work planned. Below is a list of some ideas which were never completed.
See [Contributing](#contributing) if you'd like to help.

- Engine
  - Target more G-code controllers with different formats and requirements.
  - More options for effective edge, nose, and tail. (MonkeyCAM v3, for
    instance, had 6 tail options.)
  - Soften the transition between edge and
    nose/tail. (You can work around this with a sandblock and two min of
    your time for now.)
  - Read the overall board shape from other CAD programs (DXF at least).
  - Mold design and program to cut molds.
  - Roughing passes for the core profile.
  - Output the overall shape, insert locations, etc. to PDF/DXF for
    printing or use in other CAD programs.
  - Adapt for other kinds of 'boards': kite boards, skate boards, etc.
- Web App
  - Improve input UI, remove need to use JSON.
  - Improve the results UI, remove the need for the overview HTML currently generated.
  - Worker reliability and scaling: retry, allow multiple workers again.

## Dependencies

MonkeyCAM engine currently depends on the following packages:

- Mac build: Mac OSX 15.6.1 Sequoia with the system compiler, Apple clang version 17.0.0 (clang-1700.3.19.1).
- Ubuntu build: ubuntu 22.04 LTS with gcc (Ubuntu 11.4.0-1ubuntu1~22.04.2) 11.4.0.
- CMake 3.22 See http://www.cmake.org/ or install via something like Ports.
- Clipper 5.1.6. See
  http://sourceforge.net/projects/polyclipping/. The C++ version is copied in the clipper directory.

The web app currently depends on the following packages:

- Node.js, v8.15.0 for the cloud functions and client, and 24.11.1 for the worker.
- See the package.json files in the respective source directories for further dependencies. The major ones are React and Bootstrap on the UI side, and Google's cloud infra (Storage, Datastore, Pubsub) on the server side.
- The web app is hosted on Google's cloud infra: App Engine, Cloud Functions, and Compute Engine.

## Building the Linux engine with Docker

You can build the Linux AMD64 native engine on macOS (or any machine with Docker) without installing a local toolchain. The `engine/Dockerfile` produces a small runtime image and makes it easy to copy the compiled binary back to your host:

```
docker build --platform=linux/amd64 -t monkeycam-engine-builder -f engine/Dockerfile .
docker create --name monkeycam-build monkeycam-engine-builder
docker cp monkeycam-build:/opt/monkeycam ./monkeycam-dist
docker rm monkeycam-build
```

The `./monkeycam-dist/bin` directory will contain the `MonkeyCAM` binary and supporting files. You can also run the binary inside the image directly:

```
docker run --rm monkeycam-engine-builder --help
```

## Contributing

If you're a software developer who is into ski and snowboard
construction or CAM and you're interested in helping out on MonkeyCAM
then let's chat. Open up an issue with what you want to work on and
we'll work out the details there, or fork the repo, make some changes
and put up a pull request!

- [Design Overview](https://github.com/mikemag/MonkeyCAM/wiki/Design-Overview): details on the internal design of MonkeyCAM and how it creates shapes and G-Code programs.
- [Current issues and enhancements](https://github.com/mikemag/MonkeyCAM/issues)
