find_package(JNI)

add_library(retro-j SHARED library.c library.h org_qw3rtrun_libretro_EnvironmentNative.h)