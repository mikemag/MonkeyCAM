# clipper

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-g -std=c++11")
endif()

set(CLIPPER_SRCS ../../clipper_ver5.1.6/cpp/clipper.cpp)
add_library(clipper ${CLIPPER_SRCS})
