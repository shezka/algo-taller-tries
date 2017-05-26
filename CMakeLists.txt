cmake_minimum_required(VERSION 3.6)
project(tallerTries)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    base/DiccString.hpp
    base/tests.cpp)

add_executable(tallerTries ${SOURCE_FILES})
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(runTests base/tests.cpp base/DiccString.hpp)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)