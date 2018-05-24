find_package (Boost COMPONENTS unit_test_framework REQUIRED)
include_directories (${TEST_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})
add_executable (retro-j-test test.cpp)
target_link_libraries (retro-j-test retro-j)