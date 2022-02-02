#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif
#include <map>

/* #include "aircontroller.hpp" */
/* #include "ray_animation.hpp" */
/* #include "splash.hpp" */
/* #include "credit.hpp" */
#include "control.hpp"
#include "data.hpp"
#include "gui.hpp"
#include "logic.hpp"
#include "raylib.h"

int main(int argc, char *argv[]) {
    // load game assets
    data::plane_sprites = control::state::loadPlaneSprites();
    data::digit_sprites = control::state::loadDigitSprites();
    data::background_sprites = control::state::loadBackgroundSprites();
    data::odot_sprite = control::state::loadODotSprite();
    data::flag_sprites = control::state::loadFlagSprites();
    /* data::apron_sprites = control::state::loadApronSprites(); */

    InitWindow(data::screen_width, data::screen_height, "Air Controller");

    // register Scene objects with their enum instance
    /* scene_manager.scenes[RAYANIM] = make_unique<RayAnimation>(); */
    /* scene_manager.scenes[SPLASH] = make_unique<Splash>(); */
    data::scene_manager.scenes[data::AIRCONTROLLER] =
        make_unique<AirControllerScene>();

    /* scene_manager.scenes[CREDIT] = make_unique<CreditScene>(); */

    // set and initialize the initial Scene
    scene_manager.init(data::AIRCONTROLLER);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(step, FPS, 1);
#else
    SetTargetFPS(fps);

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        data::scene_manager.Step();
    }
#endif
    // De-Initialization
    data::scene_manager.unload();
    control::state::unloadPlaneSprites();
    control::state::unloadDigitSprites();
    control::state::unloadBackgroundSprites();
    control::state::unloadFlagSprites();
    control::state::unloadApronSprites();
    control::state::unloadODotSprite();
    CloseWindow();  // Close window and OpenGL context
    return 0;
}
