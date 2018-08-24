cmake_minimum_required(VERSION 3.9)
project(retro-j-test)

find_package(JNI REQUIRED)

set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR})

enable_testing()

include_directories(${PROJECT_BINARY_DIR} ${PROJECT_INCLUDE_DIR} ${JNI_INCLUDE_DIRS})

add_executable (retro-j-test test.cpp)

link_directories(${CMAKE_BINARY_DIR} ${PROJECT_BINARY_DIR})

target_link_libraries (retro-j-test retro-j gtest gmock ${JNI_LIBRARIES})