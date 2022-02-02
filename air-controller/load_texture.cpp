#include "control.hpp"

array<data::Sprite, 11> control::state::loadPlaneSprites() {
    array<data::Sprite, 11> out{};
    for (int i = 0; i < 11; ++i) {
        out[i] = data::Sprite{
            .id = i,
            .texture = LoadTexture(TextFormat("resources/ship_%i.png"))};
    }
    return out;
}
array<data::Sprite, 10> control::state::loadDigitSprites() {
    array<data::Sprite, 10> out{};
    for (int i = 0; i < 10; ++i) {
        out[i] = data::Sprite{
            .id = i,
            .texture = LoadTexture(TextFormat("resources/digit_%i.png"))};
    }
    return out;
}
array<data::Sprite, 1> control::state::loadBackgroundSprites() {
    array<data::Sprite, 1> out{};
    for (int i = 0; i < 1; ++i) {
        out[i] = data::Sprite{
            .id = i, .texture = LoadTexture(TextFormat("resources/ito.png"))};
    }
    return out;
}
data::Sprite control::state::loadODotSprite() {
    return data::Sprite{.texture = LoadTexture("resources/odot.png")};
}
array<data::Sprite, 1> control::state::loadFlagSprites() {
    return {data::Sprite{.texture = LoadTexture("resources/flag.png")}};
}
array<data::Sprite, 1> control::state::loadApronSprites() {
    return {data::Sprite{.texture = LoadTexture("resources/apron.png")}};
}
void control::state::unloadPlaneSprites() {
    for (auto &s : data::plane_sprites) {
        UnloadTexture(s.texture);
    }
}
void control::state::unloadDigitSprites() {
    for (auto &s : data::digit_sprites) {
        UnloadTexture(s.texture);
    }
}
void control::state::unloadBackgroundSprites() {
    for (auto &s : data::background_sprites) {
        UnloadTexture(s.texture);
    }
}
void control::state::unloadODotSprite() {
    UnloadTexture(data::odot_sprite.texture);
}
void control::state::unloadFlagSprites() {
    for (auto &sprite : data::flag_sprites) {
        UnloadTexture(sprite.texture);
    }
}
void control::state::unloadApronSprites() {
    for (auto &sprite : data::apron_sprites) {
        UnloadTexture(sprite.texture);
    }
}
