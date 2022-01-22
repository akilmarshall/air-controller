#include "escape.hpp"

// EscapeScene implemenation
// private
// public
void EscapeScene::init() {
    done_ = false;

    frame_counter = 0;

    // camera initial parameters
    camera = {0};
    camera.position = Vector3{4.f, 2.f, 4.f};
    camera.target = Vector3{0.f, 1.8f, 0.f};
    camera.up = Vector3{0.f, 1.f, 0.f};
    camera.fovy = 60.f;
    camera.projection = CAMERA_PERSPECTIVE;
    SetCameraMode(camera, CAMERA_FIRST_PERSON);

    // camera state context data
    CAMERA.targetDistance = 0.f;
    CAMERA.playerEyesPosition = 1.85f;
    CAMERA.angle = {0};
    CAMERA.previousMousePosition = {0};
    CAMERA.moveControl = {'W', 'S', 'D', 'A', 'E', 'Q'};
    CAMERA.smoothZoomControl = 341;
    CAMERA.altControl = 342;
    CAMERA.panControl = 2;

    PLAYER_MOVEMENT_SENSITIVITY = 20.f;
    CAMERA_MOUSE_MOVE_SENSITIVITY = 0.003f;
    MIN_CLAMP = 89.f;
    MAX_CLAMP = -89.f;
    STEP_TRIGONOMETRIC_DIVIDER = 8.f;
    STEP_DIVIDER = 30.f;
    WAVING_DIVIDER = 200.f;
    PANNING_DIVIDER = 5.1f;
}
void EscapeScene::update() {
    frame_counter++;
    if (IsMouseButtonPressed(0)) {
        /* done_ = true; */
    }

    UpdateCamera(&camera);
    // update camera
    // data setup
    /*
    int swingCounter = 0;
    std::array<bool, 6> direction = {
        IsKeyDown(CAMERA.moveControl[MOVE_FRONT]),
        IsKeyDown(CAMERA.moveControl[MOVE_BACK]),
        IsKeyDown(CAMERA.moveControl[MOVE_RIGHT]),
        IsKeyDown(CAMERA.moveControl[MOVE_LEFT]),
        IsKeyDown(CAMERA.moveControl[MOVE_UP]),
        IsKeyDown(CAMERA.moveControl[MOVE_DOWN]),
    };
    auto mousePositionDelta = Vector2{0.f, 0.f};
    auto mousePosition = GetMousePosition();
    mousePositionDelta.x = mousePosition.x - CAMERA.previousMousePosition.x;
    mousePositionDelta.y = mousePosition.y - CAMERA.previousMousePosition.y;
    CAMERA.previousMousePosition = mousePosition;
    // first person control implemenation
    camera.position.x += (sinf(CAMERA.angle.x) * direction[MOVE_BACK] -
                          sinf(CAMERA.angle.x) * direction[MOVE_FRONT] -
                          cosf(CAMERA.angle.x) * direction[MOVE_LEFT] +
                          cosf(CAMERA.angle.x) * direction[MOVE_RIGHT]) /
                         PLAYER_MOVEMENT_SENSITIVITY;

    camera.position.y +=
        (sinf(CAMERA.angle.y) * direction[MOVE_FRONT] -
         sinf(CAMERA.angle.y) * direction[MOVE_BACK] +
         1.f * direction[MOVE_UP] - 1.f * direction[MOVE_DOWN]) /
        PLAYER_MOVEMENT_SENSITIVITY;

    camera.position.z += (cosf(CAMERA.angle.x) * direction[MOVE_BACK] -
                          cosf(CAMERA.angle.x) * direction[MOVE_FRONT] +
                          sinf(CAMERA.angle.x) * direction[MOVE_LEFT] -
                          sinf(CAMERA.angle.x) * direction[MOVE_RIGHT]) /
                         PLAYER_MOVEMENT_SENSITIVITY;

    // Camera orientation calculation
    CAMERA.angle.x += (mousePositionDelta.x * -CAMERA_MOUSE_MOVE_SENSITIVITY);
    CAMERA.angle.y += (mousePositionDelta.y * -CAMERA_MOUSE_MOVE_SENSITIVITY);

    // Angle clamp
    if (CAMERA.angle.y > MIN_CLAMP * DEG2RAD)
        CAMERA.angle.y = MIN_CLAMP * DEG2RAD;
    else if (CAMERA.angle.y < MAX_CLAMP * DEG2RAD)
        CAMERA.angle.y = MAX_CLAMP * DEG2RAD;

    // Calculate translation matrix
    Matrix matTranslation = {
        1.f, 0.f, 0.f, 0.f, 0.f, 1.f,
        0.f, 0.f, 0.f, 0.f, 1.f, (CAMERA.targetDistance / PANNING_DIVIDER),
        0.f, 0.f, 0.f, 1.f};

    // Calculate rotation matrix
    Matrix matRotation = {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
                          0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};

    float cosz = cosf(0.f);
    float sinz = sinf(0.f);
    float cosy = cosf(-(PI * 2 - CAMERA.angle.x));
    float siny = sinf(-(PI * 2 - CAMERA.angle.x));
    float cosx = cosf(-(PI * 2 - CAMERA.angle.y));
    float sinx = sinf(-(PI * 2 - CAMERA.angle.y));

    matRotation.m0 = cosz * cosy;
    matRotation.m4 = (cosz * siny * sinx) - (sinz * cosx);
    matRotation.m8 = (cosz * siny * cosx) + (sinz * sinx);
    matRotation.m1 = sinz * cosy;
    matRotation.m5 = (sinz * siny * sinx) + (cosz * cosx);
    matRotation.m9 = (sinz * siny * cosx) - (cosz * sinx);
    matRotation.m2 = -siny;
    matRotation.m6 = cosy * sinx;
    matRotation.m10 = cosy * cosx;

    // Multiply translation and rotation matrices
    Matrix matTransform = {0};
    matTransform.m0 = matTranslation.m0 * matRotation.m0 +
                      matTranslation.m1 * matRotation.m4 +
                      matTranslation.m2 * matRotation.m8 +
                      matTranslation.m3 * matRotation.m12;
    matTransform.m1 = matTranslation.m0 * matRotation.m1 +
                      matTranslation.m1 * matRotation.m5 +
                      matTranslation.m2 * matRotation.m9 +
                      matTranslation.m3 * matRotation.m13;
    matTransform.m2 = matTranslation.m0 * matRotation.m2 +
                      matTranslation.m1 * matRotation.m6 +
                      matTranslation.m2 * matRotation.m10 +
                      matTranslation.m3 * matRotation.m14;
    matTransform.m3 = matTranslation.m0 * matRotation.m3 +
                      matTranslation.m1 * matRotation.m7 +
                      matTranslation.m2 * matRotation.m11 +
                      matTranslation.m3 * matRotation.m15;
    matTransform.m4 = matTranslation.m4 * matRotation.m0 +
                      matTranslation.m5 * matRotation.m4 +
                      matTranslation.m6 * matRotation.m8 +
                      matTranslation.m7 * matRotation.m12;
    matTransform.m5 = matTranslation.m4 * matRotation.m1 +
                      matTranslation.m5 * matRotation.m5 +
                      matTranslation.m6 * matRotation.m9 +
                      matTranslation.m7 * matRotation.m13;
    matTransform.m6 = matTranslation.m4 * matRotation.m2 +
                      matTranslation.m5 * matRotation.m6 +
                      matTranslation.m6 * matRotation.m10 +
                      matTranslation.m7 * matRotation.m14;
    matTransform.m7 = matTranslation.m4 * matRotation.m3 +
                      matTranslation.m5 * matRotation.m7 +
                      matTranslation.m6 * matRotation.m11 +
                      matTranslation.m7 * matRotation.m15;
    matTransform.m8 = matTranslation.m8 * matRotation.m0 +
                      matTranslation.m9 * matRotation.m4 +
                      matTranslation.m10 * matRotation.m8 +
                      matTranslation.m11 * matRotation.m12;
    matTransform.m9 = matTranslation.m8 * matRotation.m1 +
                      matTranslation.m9 * matRotation.m5 +
                      matTranslation.m10 * matRotation.m9 +
                      matTranslation.m11 * matRotation.m13;
    matTransform.m10 = matTranslation.m8 * matRotation.m2 +
                       matTranslation.m9 * matRotation.m6 +
                       matTranslation.m10 * matRotation.m10 +
                       matTranslation.m11 * matRotation.m14;
    matTransform.m11 = matTranslation.m8 * matRotation.m3 +
                       matTranslation.m9 * matRotation.m7 +
                       matTranslation.m10 * matRotation.m11 +
                       matTranslation.m11 * matRotation.m15;
    matTransform.m12 = matTranslation.m12 * matRotation.m0 +
                       matTranslation.m13 * matRotation.m4 +
                       matTranslation.m14 * matRotation.m8 +
                       matTranslation.m15 * matRotation.m12;
    matTransform.m13 = matTranslation.m12 * matRotation.m1 +
                       matTranslation.m13 * matRotation.m5 +
                       matTranslation.m14 * matRotation.m9 +
                       matTranslation.m15 * matRotation.m13;
    matTransform.m14 = matTranslation.m12 * matRotation.m2 +
                       matTranslation.m13 * matRotation.m6 +
                       matTranslation.m14 * matRotation.m10 +
                       matTranslation.m15 * matRotation.m14;
    matTransform.m15 = matTranslation.m12 * matRotation.m3 +
                       matTranslation.m13 * matRotation.m7 +
                       matTranslation.m14 * matRotation.m11 +
                       matTranslation.m15 * matRotation.m15;

    camera.target.x = camera.position.x - matTransform.m12;
    camera.target.y = camera.position.y - matTransform.m13;
    camera.target.z = camera.position.z - matTransform.m14;

    // If movement detected (some key pressed), increase swinging
    for (int i = 0; i < 6; i++)
        if (direction[i]) {
            swingCounter++;
            break;
        }

    // Camera position update
    // NOTE: On CAMERA_FIRST_PERSON player Y-movement is limited to player 'eyes
    // position'
    camera.position.y =
        CAMERA.playerEyesPosition -
        sinf(swingCounter / STEP_TRIGONOMETRIC_DIVIDER) / STEP_DIVIDER;

    camera.up.x =
        sinf(swingCounter / (STEP_TRIGONOMETRIC_DIVIDER * 2)) / WAVING_DIVIDER;
    camera.up.z =
        -sinf(swingCounter / (STEP_TRIGONOMETRIC_DIVIDER * 2)) / WAVING_DIVIDER;
    */
}
void EscapeScene::draw() {
    ClearBackground(RAYWHITE);
    BeginMode3D(camera);
    auto cubePos = Vector3{};
    DrawCube(cubePos, 2.f, 2.f, 2.f, RED);
    DrawCubeWires(cubePos, 2.f, 2.f, 2.f, MAROON);
    DrawGrid(10, 1.f);
    EndMode3D();
    /* DrawText("2d drawing now", 0, 0, 20, BLACK); */
    DrawFPS(0, 0);
}
void EscapeScene::unload() {}
bool EscapeScene::done() { return done_; }
