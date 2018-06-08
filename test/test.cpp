//
// Created by qw3rtrun on 23.05.18.
//

#define BOOST_TEST_MODULE libretro-j-test

#include <boost/test/included/unit_test.hpp>
#include "src/retro-j.h"
#include "src/org_qw3rtrun_libretro_cb_EnvironmentNative.h"


bool stub_environment(unsigned cmd, void *data) {
    return false;
}

BOOST_AUTO_TEST_CASE(CheckRetroStub) {
    retro_init();
    retro_set_environment(&stub_environment);
    struct retro_game_info info = {.path = "../libretro.jar"};
    bool loaded = retro_load_game(&info);
    BOOST_CHECK_EQUAL(loaded, true);
    retro_deinit();
}

//int main() {
// startJvm();
//    return 0;
//}
