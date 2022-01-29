#include "credit.hpp"

// CreditScene implemenation
// private
// public
void CreditScene::init() {
    done_ = false;
    frame_counter = 0;
}
void CreditScene::update() {
    frame_counter++;
    if (IsMouseButtonPressed(0) && frame_counter > (60 * 1.5)) {
        done_ = true;
    }
}
void CreditScene::draw() {
    ClearBackground(RAYWHITE);
    auto x = GetScreenWidth() / 5;
    auto y = GetScreenHeight() / 5;

    DrawText("Developed by Akil Marshall\n  Thank you for playing!", x, y, 40,
             BLACK);
    if (frame_counter > (60 * 1.5)) {
        DrawText("click to continue", (GetScreenWidth() / 2) - 70,
                 (GetScreenHeight() / 2), 20, DARKGRAY);
    }
}
void CreditScene::unload() {}
bool CreditScene::done() { return done_; }
