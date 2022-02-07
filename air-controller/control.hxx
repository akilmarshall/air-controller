/* Accidental
 * state and control
 * performance optimization hints with accidental state and it's consequences
 */
#ifndef CONTROL_HXX
#define CONTROL_HXX
/* #include "api.hxx" */
#include "data.hxx"
#include "logic.hxx"
#include "raylib.h"
namespace control {
namespace state {
void loadTextures();
/* inline void loadBG() { */
/*     data::textures["ito"] = LoadTexture("resources/ito.png"); */
/* } */
inline Texture2D getBG() { return data::textures["ito"]; }
inline void unloadTextures() {
    for (auto &[k, v] : data::textures) {
        UnloadTexture(v);
    }
}
void processUserInput();
}  // namespace state
}  // namespace control
#endif
