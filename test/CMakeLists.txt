find_package (Boost COMPONENTS unit_test_framework REQUIRED)
find_package(JNI REQUIRED)
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${PROJECT_BINARY_DIR} ${Boost_INCLUDE_DIRS} ${JNI_INCLUDE_DIRS})

add_executable (retro-j-test test.cpp)

include_directories(${PROJECT_INCLUDE_DIR})

target_link_libraries (retro-j-test retro-j ${JNI_LIBRARIES})