cmake_minimum_required(VERSION 2.8)
project(MonkeyCAM)

set(MonkeyCAM_VERSION_MAJOR 4)
set(MonkeyCAM_VERSION_MINOR 0)
set(MonkeyCAM_VERSION_PATCH 1)
execute_process(
  COMMAND git log --pretty=format:%h -n 1 OUTPUT_VARIABLE MonkeyCAM_GIT_REV
)
execute_process(
  COMMAND git ls-files -m OUTPUT_VARIABLE MonkeyCAM_GIT_CHANGED
)
if (MonkeyCAM_GIT_CHANGED)
  set(MonkeyCAM_GIT_CHANGED "-PendingChanges")
endif()
set(MonkeyCAM_VERSION_ID "${MonkeyCAM_GIT_REV}${MonkeyCAM_GIT_CHANGED}")
set(MonkeyCAM_VERSION ${MonkeyCAM_VERSION_MAJOR}.${MonkeyCAM_VERSION_MINOR}.${MonkeyCAM_VERSION_PATCH}-${MonkeyCAM_VERSION_ID})

configure_file(
  "${PROJECT_SOURCE_DIR}/util/MonkeyCAMConfig.h.in"
  "${PROJECT_SOURCE_DIR}/util/MonkeyCAMConfig.h"
)

set(CMAKE_INSTALL_PREFIX "Dist-${MonkeyCAM_VERSION}")

add_subdirectory(util)
add_subdirectory(clipper)
add_subdirectory(core)
add_subdirectory(cli)
#add_subdirectory(test)

install(FILES "${PROJECT_BINARY_DIR}/LICENSE" DESTINATION .)
install(FILES "${PROJECT_BINARY_DIR}/README.md" DESTINATION .)
install(FILES "${PROJECT_BINARY_DIR}/grid.svg" DESTINATION .)
install(FILES "${PROJECT_BINARY_DIR}/board-def.json" DESTINATION example)
install(FILES "${PROJECT_BINARY_DIR}/machine-tool-def.json" DESTINATION example)
