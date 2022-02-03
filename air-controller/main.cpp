#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "api.hpp"
#include "control.hpp"
#include "data.hpp"
#include "logic.hpp"
#include "raylib.h"

int main(int argc, char *argv[]) {
    InitWindow(data::screen_width, data::screen_height, "Air Controller");

    // load game assets
    data::plane_sprites = control::state::loadPlaneSprites();
    data::digit_sprites = control::state::loadDigitSprites();
    data::background_sprites = control::state::loadBackgroundSprites();
    data::odot_sprite = control::state::loadODotSprite();
    data::flag_sprites = control::state::loadFlagSprites();
    /* data::apron_sprites = control::state::loadApronSprites(); */

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(api::scene::step, data::fps, 1);
#else
    SetTargetFPS(data::fps);

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        api::scene::step();
    }
#endif
    // De-Initialization
    control::state::unloadPlaneSprites();
    control::state::unloadDigitSprites();
    control::state::unloadBackgroundSprites();
    control::state::unloadFlagSprites();
    control::state::unloadApronSprites();
    control::state::unloadODotSprite();
    CloseWindow();  // Close window and OpenGL context
    return 0;
}
