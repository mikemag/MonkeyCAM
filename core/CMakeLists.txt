# core

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++11")
endif()

include_directories(${MonkeyCAM_SOURCE_DIR}/util)
include_directories(${MonkeyCAM_SOURCE_DIR}/clipper)

set(MONKEYCAM_CORE_SRCS
    arc-path.cpp
    bezier-path.cpp
    board-profile.cpp
    ellipse-path.cpp
    gcode-writer.cpp
    insert-pack.cpp
    machine.cpp
    mcfixed.cpp
    paths.cpp
    point.cpp
    shape-parts.cpp
    shapes.cpp
    )
add_library(core ${MONKEYCAM_CORE_SRCS})
