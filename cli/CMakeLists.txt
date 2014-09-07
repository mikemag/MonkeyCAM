# cli
include_directories(${MonkeyCAM_SOURCE_DIR}/util)
include_directories(${MonkeyCAM_SOURCE_DIR}/core)
include_directories(/opt/local/include) # For boost

link_directories(${MonkeyCAM_BINARY_DIR}/util)
link_directories(${MonkeyCAM_BINARY_DIR}/core)
link_directories(${MonkeyCAM_BINARY_DIR}/clipper)

SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)

add_executable(MonkeyCAM
               monkey-cam.cpp
               )

target_link_libraries(MonkeyCAM util core clipper ${Boost_LIBRARIES})

set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++11")

install(CODE "
  message(\"Signing ${CMAKE_CURRENT_BINARY_DIR}/MonkeyCAM...\")
  execute_process(
    COMMAND codesign -s \"Developer ID Application\" ${CMAKE_CURRENT_BINARY_DIR}/MonkeyCAM
  )
  message(\"Verifying ${CMAKE_CURRENT_BINARY_DIR}/MonkeyCAM...\")
  execute_process(
    COMMAND codesign --display --verbose=4 ${CMAKE_CURRENT_BINARY_DIR}/MonkeyCAM
  )
  message(\"Done signing.\")
")

install(TARGETS MonkeyCAM DESTINATION bin/MacOS)
