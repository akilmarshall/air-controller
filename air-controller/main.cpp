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
    plane_sprites = loadPlaneSprites();
    digit_sprites = loadDigitSprites();
    background_sprites = loadBackgroundSprites();
    odot_sprite = loadODotSprite();
    flag_sprite = loadFlagSprite();
    // apron_sprite = loadApronSprite();

    InitWindow(screen_width, screen_height, "Air Controller");

    // register Scene objects with their enum instance
    /* scene_manager.scenes[RAYANIM] = make_unique<RayAnimation>(); */
    /* scene_manager.scenes[SPLASH] = make_unique<Splash>(); */
    scene_manager.scenes[AIRCONTROLLER] = make_unique<AirControllerScene>();

    /* scene_manager.scenes[CREDIT] = make_unique<CreditScene>(); */

    // set and initialize the initial Scene
    scene_manager.init(AIRCONTROLLER);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(step, FPS, 1);
#else
    SetTargetFPS(fps);

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        scene_manager.Step();
    }
#endif
    // De-Initialization
    scene_manager.unload();
    unloadPlaneSprites();
    unloadDigitSprites();
    unloadBackgroundSprites();
    unloadODotSprite();
    CloseWindow();  // Close window and OpenGL context
    return 0;
}
