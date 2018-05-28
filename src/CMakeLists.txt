find_package(JNI REQUIRED)

include_directories(${JNI_INCLUDE_DIRS})

set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}")

add_library(retro-j SHARED library.c library.h org_qw3rtrun_libretro_EnvironmentNative.h)

target_link_libraries(retro-j ${JNI_LIBRARIES})
