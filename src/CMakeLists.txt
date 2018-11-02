find_package(JNI REQUIRED)

include_directories(${JNI_INCLUDE_DIRS})

set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR})

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}")

add_library(retro-j SHARED
        retro-j.c retro-j.h
        libretro.h
        classloader.c
        classloader.h
        org_qw3rtrun_libretro_cb_EnvironmentNative.c
        org_qw3rtrun_libretro_cb_EnvironmentNative.h
        org_qw3rtrun_libretro_cb_LogCallbackNative.c
        org_qw3rtrun_libretro_cb_LogCallbackNative.h)

target_link_libraries(retro-j ${JNI_LIBRARIES})
