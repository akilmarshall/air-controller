#include "ray_animation.hpp"

void RayAnimation::init() {
    logoX = (GetScreenWidth() / 2) - 128;
    logoY = (GetScreenHeight() / 2) - 128;
    frame_counter = 0;
    letterCount = 0;
    top = 16;
    left = 16;
    bottom = 16;
    right = 16;
    state = 0;
    done_ = false;
}
void RayAnimation::update() {
    switch (state) {
        case 0: {
            ++frame_counter;
            if (frame_counter == 120) {
                state = 1;
                frame_counter = 0;
            }
        } break;
        case 1: {
            top += 4;
            left += 4;
            if (top == 256) {
                state = 2;
            }
        } break;
        case 2: {
            bottom += 4;
            right += 4;
            if (bottom == 256) {
                state = 3;
            }
        } break;
        case 3: {
            ++frame_counter;
            if (letterCount < 10 && frame_counter == 8) {
                ++letterCount;
                frame_counter = 0;
            }
            if (letterCount >= 10 && frame_counter > 30) {
                done_ = true;
            }
        } break;
    }

    if (IsMouseButtonPressed(0) || IsMouseButtonPressed(1) ||
        IsKeyPressed(KEY_SPACE)) {
        done_ = true;
    }
}
void RayAnimation::draw() {
    ClearBackground(RAYWHITE);
    switch (state) {
        case 0: {
            if ((frame_counter / 15) % 2) {
                DrawRectangle(logoX, logoY, 16, 16, BLACK);
            }
        } break;
        case 1: {
            DrawRectangle(logoX, logoY, top, 16, BLACK);
            DrawRectangle(logoX, logoY, 16, left, BLACK);
        } break;
        case 2: {
            DrawRectangle(logoX, logoY, top, 16, BLACK);
            DrawRectangle(logoX, logoY, 16, left, BLACK);

            DrawRectangle(logoX + 240, logoY, 16, right, BLACK);
            DrawRectangle(logoX, logoY + 240, bottom, 16, BLACK);
        } break;
        case 3: {
            DrawRectangle(logoX, logoY, top, 16, BLACK);
            DrawRectangle(logoX, logoY + 16, 16, left - 32, BLACK);

            DrawRectangle(logoX + 240, logoY + 16, 16, right - 32, BLACK);
            DrawRectangle(logoX, logoY + 240, bottom, 16, BLACK);

            DrawRectangle(GetScreenWidth() / 2 - 112,
                          GetScreenHeight() / 2 - 112, 224, 224, RAYWHITE);

            DrawText(TextSubtext("raylib", 0, letterCount),
                     GetScreenWidth() / 2 - 44, GetScreenHeight() / 2 + 48, 50,
                     BLACK);
        } break;
    }
}
void RayAnimation::unload() {}
bool RayAnimation::done() { return done_; }
