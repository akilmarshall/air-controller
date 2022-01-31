#include "control.hpp"

array<Sprite, 11> loadPlaneSprites() {
    array<Sprite, 11> out{};
    for (int i = 0; i < 11; ++i) {
        out[i] =
            Sprite{.id = i,
                   .texture = LoadTexture(TextFormat("resources/ship_%i.png"))};
    }
    return out;
}
array<Sprite, 10> loadDigitSprites() {
    array<Sprite, 10> out{};
    for (int i = 0; i < 10; ++i) {
        out[i] = Sprite{
            .id = i,
            .texture = LoadTexture(TextFormat("resources/digit_%i.png"))};
    }
    return out;
}
array<Sprite, 1> loadBackgroundSprites() {
    array<Sprite, 1> out{};
    for (int i = 0; i < 1; ++i) {
        out[i] = Sprite{
            .id = i, .texture = LoadTexture(TextFormat("resources/ito.png"))};
    }
    return out;
}
Sprite loadODotSprite() {
    return Sprite{.texture = LoadTexture("resources/odot.png")};
}
Sprite loadFlagSprite() {
    return Sprite{.texture = LoadTexture("resources/flag.png")};
}
Sprite loadApronSprite() {
    return Sprite{.texture = LoadTexture("resources/apron.png")};
}
void unloadPlaneSprites() {
    for (auto &s : plane_sprites) {
        UnloadTexture(s.texture);
    }
}
void unloadDigitSprites() {
    for (auto &s : digit_sprites) {
        UnloadTexture(s.texture);
    }
}
void unloadBackgroundSprites() {
    for (auto &s : background_sprites) {
        UnloadTexture(s.texture);
    }
}
void unloadODotSprite() { UnloadTexture(odot_sprite.texture); }
void unloadFlagSprite() { UnloadTexture(flag_sprite.texture); }
