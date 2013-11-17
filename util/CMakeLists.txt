# util

set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++11")

set(MONKEYCAM_UTIL_SRCS
    basic-math.cpp
    svg-writer.cpp
    )
add_library(util ${MONKEYCAM_UTIL_SRCS})
