#ifndef SPLASH_HPP
#define SPLASH_HPP
#include <iostream>
#include <string>

#include "raylib.h"
#include "raymath.h"
#include "scene.hpp"

class Splash : public Scene {
   private:
    int frame_counter = 0;
    int t;  // animation plays for t seconds
    bool done_ = false;
    void f(int xlen, int ylen, float xspeed, float yspeed, float a, float b,
           int xoff, int yoff, Color color);

   public:
    Splash();
    void init();
    void draw();
    void unload();
    void update();
    bool done();
};
#endif
