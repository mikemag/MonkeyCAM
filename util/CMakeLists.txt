# util

if(UNIX)
  include_directories(/opt/local/include) # For boost
  set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++11")
elseif(MINGW)
  include_directories(../../boost_1_64_0)
  set(CMAKE_CXX_FLAGS "-g -Werror -std=gnu++11") # NB Boost warnings with GCC
endif(UNIX)

set(MONKEYCAM_UTIL_SRCS
    activity-emitter.cpp
    basic-math.cpp
    config.cpp
    )
add_library(util ${MONKEYCAM_UTIL_SRCS})
