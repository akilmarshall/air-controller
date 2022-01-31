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

// control
void processUserInput();

// state
array<Sprite, 11> loadPlaneSprites();
array<Sprite, 10> loadDigitSprites();
array<Sprite, 1> loadBackgroundSprites();
Sprite loadODotSprite();
Sprite loadFlagSprite();
Sprite loadApronSprite();
void unloadPlaneSprites();
void unloadDigitSprites();
void unloadBackgroundSprites();
void unloadODotSprite();
void unloadFlagSprite();
void unloadApronSprite();
#endif
