/* Accidental
 * state and control
 * performance optimization hints with accidental state and it's consequences
 */
#ifndef CONTROL_HPP
#define CONTROL_HPP
#include <array>

#include "api.hpp"
#include "data.hpp"
#include "logic.hpp"
#include "raylib.h"

using std::array;
namespace control {

// state
namespace state {
void processUserInput();
array<data::Sprite, 11> loadPlaneSprites();
array<data::Sprite, 10> loadDigitSprites();
array<data::Sprite, 1> loadBackgroundSprites();
array<data::Sprite, 1> loadApronSprites();
array<data::Sprite, 1> loadFlagSprites();
data::Sprite loadODotSprite();
void unloadPlaneSprites();
void unloadDigitSprites();
void unloadBackgroundSprites();
void unloadFlagSprites();
void unloadApronSprites();
void unloadODotSprite();
}  // namespace state
}  // namespace control
#endif
