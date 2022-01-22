#ifndef ESCAPE_HPP
#define ESCAPE_HPP
#include <array>
#include <cmath>

#include "raylib.h"
#include "scene.hpp"

enum CameraMove {
    MOVE_FRONT,
    MOVE_BACK,
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN,
};
struct CameraData {
    float targetDistance;
    float playerEyesPosition;
    Vector2 angle;
    Vector2 previousMousePosition;
    std::array<int, 6> moveControl;
    int smoothZoomControl;
    int altControl;
    int panControl;
};
class EscapeScene : public Scene {
   private:
    bool done_;
    int frame_counter;

    Camera camera;
    CameraData CAMERA;
    float PLAYER_MOVEMENT_SENSITIVITY;
    float CAMERA_MOUSE_MOVE_SENSITIVITY;
    float MIN_CLAMP;
    float MAX_CLAMP;
    float STEP_TRIGONOMETRIC_DIVIDER;
    float STEP_DIVIDER;
    float WAVING_DIVIDER;
    float PANNING_DIVIDER;

   public:
    void init();
    void update();
    void draw();
    void unload();
    bool done();
};
#endif

