#ifndef RAYLIB_ANIM_HPP
#define RAYLIB_ANIM_HPP
#include <string>

#include "raylib.h"
#include "scene.hpp"

class RayAnimation : public Scene {
   private:
    bool done_;
    int logoX;
    int logoY;
    int frame_counter;
    int letterCount;
    int top;
    int left;
    int bottom;
    int right;
    int state;

   public:
    void init();
    void update();
    void draw();
    void unload();
    bool done();
};
#endif
