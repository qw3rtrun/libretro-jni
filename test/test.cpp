//
// Created by strunov on 8/24/2018.
//

#include "gtest/gtest.h"
#include "src/retro-j.h"
#include "src/org_qw3rtrun_libretro_cb_EnvironmentNative.h"

bool stub_environment(unsigned cmd, void *data) {
    return false;
}

TEST(MAIN, workflow) {
    EXPECT_EQ(RETRO_API_VERSION, retro_api_version());
    struct retro_system_info info = {};
    retro_get_system_info(&info);
    EXPECT_STREQ("jar", info.valid_extensions);
    retro_set_environment(&stub_environment);
    struct retro_game_info game = {.path = "../libretro.jar"};
    bool loaded = retro_load_game(&game);
    EXPECT_EQ(loaded, true);
}

TEST(MAIN, duplicate) {
    EXPECT_EQ(RETRO_API_VERSION, retro_api_version());
    struct retro_system_info info = {};
    retro_get_system_info(&info);
    EXPECT_STREQ("jar", info.valid_extensions);
    retro_set_environment(&stub_environment);
    struct retro_game_info game = {.path = "../libretro.jar"};
    bool loaded = retro_load_game(&game);
    EXPECT_EQ(loaded, true);
}

TEST(SAMPLES, env) {
    char* actual = getenv("TEST_VAR");
    EXPECT_STREQ("value1", actual);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    retro_init();
    int i = RUN_ALL_TESTS();
    retro_deinit();
    return i;
}