# util

if(UNIX)
  set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++17")
elseif(MINGW)
  set(CMAKE_CXX_FLAGS "-g -Werror -std=gnu++17")
endif(UNIX)

set(MONKEYCAM_UTIL_SRCS
    activity-emitter.cpp
    basic-math.cpp
    config.cpp
    )
add_library(util ${MONKEYCAM_UTIL_SRCS})
