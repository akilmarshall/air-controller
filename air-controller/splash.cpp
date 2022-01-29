#include "splash.hpp"

// private methods
void Splash::f(int xlen, int ylen, float xspeed, float yspeed, float a, float b,
               int xoff, int yoff, Color color) {
    float x = xlen * cosf((frame_counter * xspeed) + a);
    float y = ylen * sinf((frame_counter * yspeed) + b);
    DrawCircle(x + xoff, y + yoff, 5, color);
    DrawCircle(-x + xoff, -y + yoff, 5, color);
    DrawLine(x + xoff, y + yoff, -x + xoff, -y + yoff, color);
}
// public methods
Splash::Splash() {}
void Splash::init() {
    t = 5;
    frame_counter = 0;
    done_ = false;
}
void Splash::draw() {
    f(50, 50, 0.05, 0.03, 0.0, 0.0, GetScreenWidth() / 2, 150, BLACK);
    f(50, 50, 0.01, 0.03, 0.5, 0.5, GetScreenWidth() / 2, 150, RED);
    auto text = "Unit B";
    DrawText(text, (GetScreenWidth() / 2) - (MeasureText(text, 40) / 2),
             GetScreenHeight() / 2, 40, BLACK);
    DrawText("B", (GetScreenWidth() / 2) + 36, GetScreenHeight() / 2, 40, RED);
    DrawText("Presents", (GetScreenWidth() / 2) - 70,
             (GetScreenHeight() / 2) + 40, 30, BLACK);
    ClearBackground(RAYWHITE);
}
void Splash::unload() {}
void Splash::update() {
    if (frame_counter > (60 * t)) {
        done_ = true;
    }
    // handle input allowing the player to skip the animation
    if (IsMouseButtonPressed(0) || IsMouseButtonPressed(1) ||
        IsKeyPressed(KEY_SPACE)) {
        done_ = true;
    }
    ++frame_counter;
}
bool Splash::done() { return done_; }
