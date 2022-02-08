#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "api.hxx"
#include "control.hxx"
#include "data.hxx"
#include "logic.hxx"
#include "raylib.h"

int main() {
    InitWindow(data::screen_width, data::screen_height, "Air Controller");
    // load game assets
    control::state::loadTextures();
    // define initial scene, and initialize data
    api::scene::init(data::scene::AIRCONTROLLER);
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(api::scene::step, data::fps, 1);
#else
    SetTargetFPS(data::fps);
    while (!WindowShouldClose()) {
        if (data::flights.size() > 0) {
            for (auto &flight : data::flights) {
            }
        }
        api::scene::step();
    }
#endif
    CloseWindow();
    control::state::unloadTextures();
    return 0;
}
