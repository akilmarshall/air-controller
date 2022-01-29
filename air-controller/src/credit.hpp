#ifndef CREDIT_HPP
#define CREDIT_HPP
#include "raylib.h"
#include "scene.hpp"

class CreditScene : public Scene {
   private:
    bool done_;
    int frame_counter;

   public:
    void init();
    void update();
    void draw();
    void unload();
    bool done();
};
#endif

