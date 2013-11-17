# clipper

set(CMAKE_CXX_FLAGS "-g -std=c++11")

set(CLIPPER_SRCS ../../clipper_ver5.1.6/cpp/clipper.cpp)
add_library(clipper ${CLIPPER_SRCS})
