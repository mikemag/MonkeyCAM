# test
include_directories(${MonkeyCAM_SOURCE_DIR}/core)
include_directories(${MonkeyCAM_SOURCE_DIR}/util)

if (UNIX)
    set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++17")
elseif (MINGW)
    set(CMAKE_CXX_FLAGS "-g -Werror -std=gnu++17")
endif (UNIX)

add_executable(cut-depth-test cut-depth-test.cpp)
target_link_libraries(cut-depth-test core)
add_test(NAME cut-depth-test COMMAND cut-depth-test)

add_executable(machine-config-test machine-config-test.cpp)
target_link_libraries(machine-config-test core util)
add_test(NAME machine-config-defaults
         COMMAND machine-config-test
                 ${CMAKE_CURRENT_SOURCE_DIR}/data/machine-defaults.json
                 1 1 1 true)
add_test(NAME machine-config-explicit
         COMMAND machine-config-test
                 ${CMAKE_CURRENT_SOURCE_DIR}/data/machine-explicit.json
                 2 3 4 false)
add_test(NAME machine-config-invalid-pass
         COMMAND machine-config-test
                 ${CMAKE_CURRENT_SOURCE_DIR}/data/machine-invalid-pass.json)
set_tests_properties(machine-config-invalid-pass PROPERTIES WILL_FAIL true)
add_test(NAME machine-config-negative-pass
         COMMAND machine-config-test
                 ${CMAKE_CURRENT_SOURCE_DIR}/data/machine-negative-pass.json)
set_tests_properties(machine-config-negative-pass PROPERTIES WILL_FAIL true)
add_test(NAME machine-config-fractional-pass
         COMMAND machine-config-test
                 ${CMAKE_CURRENT_SOURCE_DIR}/data/machine-fractional-pass.json)
set_tests_properties(machine-config-fractional-pass PROPERTIES WILL_FAIL true)
add_test(NAME machine-config-invalid-max-depth
         COMMAND machine-config-test
                 ${CMAKE_CURRENT_SOURCE_DIR}/data/machine-invalid-max-depth.json)
set_tests_properties(machine-config-invalid-max-depth PROPERTIES WILL_FAIL true)

find_package(Python3 QUIET COMPONENTS Interpreter)
if (Python3_Interpreter_FOUND)
    add_test(NAME max-cut-depth-regression
             COMMAND ${Python3_EXECUTABLE}
                     ${CMAKE_CURRENT_SOURCE_DIR}/verify-max-cut-depth.py
                     --monkeycam $<TARGET_FILE:MonkeyCAM>
                     --repo-root ${MonkeyCAM_SOURCE_DIR}/..)
    set_tests_properties(max-cut-depth-regression PROPERTIES TIMEOUT 120)
endif ()
