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
    if (IsMouseButtonPressed(0)) {
        done_ = true;
    }
}
void CreditScene::draw() {
    ClearBackground(RAYWHITE);
    DrawText("insert credit here..", GetScreenWidth() / 2,
             GetScreenHeight() / 2, 20, BLACK);
}
void CreditScene::unload() {}
bool CreditScene::done() { return done_; }
