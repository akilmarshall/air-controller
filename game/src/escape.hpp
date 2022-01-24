#ifndef ESCAPE_HPP
#define ESCAPE_HPP
#include <array>
#include <cmath>
#include <iostream>

#include "raylib.h"
#include "scene.hpp"

enum CameraMove {
    MOVE_FRONT = 0,
    MOVE_BACK,
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN
};
class EscapeScene : public Scene {
   private:
    bool done_;
    int frame_counter;

    Camera camera;
    float targetDistance;
    float playerEyesPosition;
    Vector2 angle;
    Vector2 previousMousePosition;
    std::array<int, 6> moveControl = {'W', 'S', 'D', 'A', 'E', 'Q'};
    float MIN_CLAMP = 89.f;
    float MAX_CLAMP = -89.f;
    float STEP_TRIGONOMETRIC_DIVIDER = 8.f;
    float STEP_DIVIDER = 30.f;
    float WAVING_DIVIDER = 200.f;
    float PANNING_DIVIDER = 5.1;
    float PLAYER_MOVEMENT_SENSITIVITY = 20.f;
    float CAMERA_MOVEMENT_SENSITIVITY = 0.003f;

    void setupCamera();
    void updateCamera();

   public:
    void init();
    void update();
    void draw();
    void unload();
    bool done();
};
#endif

