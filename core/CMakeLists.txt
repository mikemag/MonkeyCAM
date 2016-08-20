# core
include_directories(${MonkeyCAM_SOURCE_DIR}/util)
include_directories(${MonkeyCAM_SOURCE_DIR}/clipper)
if(UNIX)
  include_directories(/opt/local/include) # For boost
  set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++11")
elseif(MINGW)
  include_directories(../../boost_1_61_0)
  set(CMAKE_CXX_FLAGS "-g -Werror -std=gnu++11") # NB Boost warnings with GCC
endif(UNIX)

set(MONKEYCAM_CORE_SRCS
    arc-path.cpp
    bezier-path.cpp
    board-profile.cpp
    ellipse-path.cpp
    gcode-writer.cpp
    insert-pack.cpp
    machine.cpp
    mcfixed.cpp
    overview-writer.cpp
    paths.cpp
    point.cpp
    shape-parts.cpp
    shapes.cpp
    )
add_library(core ${MONKEYCAM_CORE_SRCS})
