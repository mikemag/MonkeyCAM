cmake_minimum_required(VERSION 3.22)
project(MonkeyCAM)

set(MonkeyCAM_VERSION_MAJOR 4)
set(MonkeyCAM_VERSION_MINOR 1)
set(MonkeyCAM_VERSION_PATCH 0)

if (NOT DEFINED GIT_BRANCH OR "${GIT_BRANCH}" STREQUAL "")
    execute_process(
            COMMAND git rev-parse --abbrev-ref HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif ()

if (NOT DEFINED GIT_COMMIT_HASH OR "${GIT_COMMIT_HASH}" STREQUAL "")
    execute_process(
            COMMAND git log -1 --format=%h
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif ()

if (NOT DEFINED GIT_COMMIT_DATE OR "${GIT_COMMIT_DATE}" STREQUAL "")
    execute_process(
            COMMAND git log -1 --format=%cD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT_DATE
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif ()

set(MonkeyCAM_VERSION ${MonkeyCAM_VERSION_MAJOR}.${MonkeyCAM_VERSION_MINOR}.${MonkeyCAM_VERSION_PATCH}-${GIT_BRANCH}-${GIT_COMMIT_HASH})

configure_file(
        "${PROJECT_SOURCE_DIR}/util/MonkeyCAMConfig.h.in"
        "${PROJECT_SOURCE_DIR}/util/MonkeyCAMConfig.h"
)

add_subdirectory(util)
add_subdirectory(clipper)
add_subdirectory(core)
add_subdirectory(cli)

install(FILES "${PROJECT_BINARY_DIR}/../LICENSE" DESTINATION .)
install(FILES "${PROJECT_BINARY_DIR}/../README.md" DESTINATION .)
install(DIRECTORY "${PROJECT_BINARY_DIR}/../example" DESTINATION .)

if (APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_FORMAT "UDZO")
    set(CPACK_DMG_VOLUME_NAME "MonkeyCAM-${MonkeyCAM_VERSION}")
else (APPLE)
    set(CPACK_GENERATOR "ZIP")
endif (APPLE)
set(CPACK_PACKAGE_VERSION_MAJOR "${MonkeyCAM_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${MonkeyCAM_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${MonkeyCAM_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "MonkeyCAM-${MonkeyCAM_VERSION}")
include(CPack)
