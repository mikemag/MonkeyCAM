# test
include_directories(${MonkeyCAM_SOURCE_DIR}/util)
include_directories(${MonkeyCAM_SOURCE_DIR}/core)

link_directories(${MonkeyCAM_BINARY_DIR}/util)
link_directories(${MonkeyCAM_BINARY_DIR}/core)

add_executable(test test.cpp)

target_link_libraries(test util core)
target_link_libraries(test -L/opt/local/lib gtest -D_THREAD_SAFE)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++11")
endif()
