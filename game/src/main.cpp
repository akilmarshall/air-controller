#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif
#include <map>

#include "credit.hpp"
#include "escape.hpp"
#include "ray_animation.hpp"
#include "raylib.h"
#include "scene.hpp"
#include "splash.hpp"

int const screenWidth = 800;
int const screenHeight = 450;
int const FPS = 60;

// define an instance for each Scene object
enum GameScene { RAYANIM, SPLASH, ESCAPE, CREDIT };
// define the scene graph
std::map<GameScene, GameScene> sceneGraph = {
    {RAYANIM, SPLASH},
    {SPLASH, ESCAPE},
    {ESCAPE, CREDIT},
    {CREDIT, ESCAPE},
};
// initialize the SceneManager
auto sm = SceneManager<GameScene>(sceneGraph, true);

// necessary void function for passing control to emscripten_set_main_loop in
// web mode
void step();

int main(int argc, char *argv[]) {
    InitWindow(screenWidth, screenHeight, "raylib cpp template");

    // register Scene objects with their enum instance
    sm.scenes[SPLASH] = make_unique<Splash>();
    sm.scenes[RAYANIM] = make_unique<RayAnimation>();
    sm.scenes[ESCAPE] = make_unique<EscapeScene>();
    sm.scenes[CREDIT] = make_unique<CreditScene>();

    // set and initialize the initial Scene
    sm.init(ESCAPE);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(step, FPS, 1);
#else
    SetTargetFPS(FPS);

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        sm.Step();
    }
#endif
    // De-Initialization
    sm.unload();
    CloseWindow();  // Close window and OpenGL context
    return 0;
}
void step() { sm.Step(); }
