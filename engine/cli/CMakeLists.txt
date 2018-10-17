# cli
include_directories(${MonkeyCAM_SOURCE_DIR}/util)
include_directories(${MonkeyCAM_SOURCE_DIR}/core)
if(UNIX)
  set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++17")
  if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
  endif(CMAKE_COMPILER_IS_GNUCXX)
elseif(MINGW)
  set(CMAKE_CXX_FLAGS "-g -Werror -std=gnu++17")
endif(UNIX)

link_directories(${MonkeyCAM_BINARY_DIR}/util)
link_directories(${MonkeyCAM_BINARY_DIR}/core)
link_directories(${MonkeyCAM_BINARY_DIR}/clipper)

add_executable(MonkeyCAM
               monkey-cam.cpp
               )

target_link_libraries(MonkeyCAM core clipper util)

if(APPLE)
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
endif(APPLE)

install(TARGETS MonkeyCAM DESTINATION bin)
if(MINGW)
install(FILES "/mingw/bin/libgcc_s_dw2-1.dll" DESTINATION bin)
install(FILES "/mingw/bin/libstdc++-6.dll" DESTINATION bin)
endif(MINGW)
