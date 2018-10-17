# clipper

if(UNIX)
  set(CMAKE_CXX_FLAGS "-g -std=c++17")
elseif(MINGW)
  set(CMAKE_CXX_FLAGS "-g -std=gnu++17")
endif(UNIX)

set(CLIPPER_SRCS clipper.cpp)
add_library(clipper ${CLIPPER_SRCS})
