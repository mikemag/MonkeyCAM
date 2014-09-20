cmake_minimum_required(VERSION 2.8)
project(MonkeyCAM)

set(MonkeyCAM_VERSION_MAJOR 4)
set(MonkeyCAM_VERSION_MINOR 0)
set(MonkeyCAM_VERSION_PATCH 2)
set(MonkeyCAM_VERSION ${MonkeyCAM_VERSION_MAJOR}.${MonkeyCAM_VERSION_MINOR}.${MonkeyCAM_VERSION_PATCH})

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

if(APPLE)
  set(CPACK_GENERATOR "DragNDrop")
  set(CPACK_DMG_FORMAT "UDZO")
  set(CPACK_DMG_VOLUME_NAME "MonkeyCAM-${MonkeyCAM_VERSION}")
else(APPLE)
  set(CPACK_GENERATOR "ZIP")
endif(APPLE)
set(CPACK_PACKAGE_VERSION_MAJOR "${MonkeyCAM_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${MonkeyCAM_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${MonkeyCAM_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "MonkeyCAM-${MonkeyCAM_VERSION}")
include(CPack)
