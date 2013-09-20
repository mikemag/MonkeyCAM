# util

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++11")
endif()

set(MONKEYCAM_UTIL_SRCS
    basic-math.cpp
    svg-writer.cpp
    )
add_library(util ${MONKEYCAM_UTIL_SRCS})
