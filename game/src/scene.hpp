#ifndef SCENE_HPP
#define SCENE_HPP

#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "raylib.h"

using std::make_unique;  // for export convenience, user ALWAYS must create
                         // smart_ptr objects since C++ does not allow
                         // polymorphism otherwise

class Scene {
    // Scene abstract interface
    // A Scene represents any "self contained" game window
    // This segregation allows each scene to handle its own graphics, input, etc
    // Some examples, Menu, Splash, Credits, Gameplay
   public:
    virtual void init() = 0;
    virtual void unload() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual bool done() = 0;
};

template <typename T>
class SceneManager {
    // Scene Manager allows a user to register points to child objects of Scene
    // and manage them with convenient methods.
    // This class is generic over T an enum type with an alternative for each
    // scene in the graph
   private:
    T scene;
    bool debug = false;
    std::map<T, T> scene_graph;
    float alpha = 0.0f;
    bool transitionFadeOut = true;
    T transitionFromScene;
    T transitionToScene;

   public:
    SceneManager(std::map<T, T> scene_graph);
    SceneManager(std::map<T, T> scene_graph, bool debug);
    std::map<T, std::unique_ptr<Scene>> scenes;
    void init(T scene);
    void unload();
    bool onTransition = false;
    void ChangeToScene(T next);
    void TransitionToScene(T next);
    void UpdateTransition();
    void DrawTransition();
    // step the current scene one frame
    void Step();
};
template <typename T>
SceneManager<T>::SceneManager(std::map<T, T> scene_graph)
    : scene_graph(scene_graph) {}
template <typename T>
SceneManager<T>::SceneManager(std::map<T, T> scene_graph, bool debug)
    : debug(debug), scene_graph(scene_graph) {}
template <typename T>
void SceneManager<T>::ChangeToScene(T next) {
    // unload the current scene
    scenes[scene]->unload();
    // load the next scene
    scenes[next]->init();
    // modify the scene data member
    scene = next;
}
template <typename T>
void SceneManager<T>::init(T scene) {
    this->scene = scene;
    scenes[scene]->init();
}
template <typename T>
void SceneManager<T>::unload() {
    scenes[scene]->unload();
}
template <typename T>
void SceneManager<T>::TransitionToScene(T next) {
    // setup transition to next scene
    onTransition = true;
    transitionFadeOut = false;
    transitionFromScene = scene;
    transitionToScene = next;
    alpha = 0.0f;
}
template <typename T>
void SceneManager<T>::UpdateTransition() {
    // step through transition (fade in/fade out)
    if (!transitionFadeOut) {
        alpha += 0.05f;
        if (alpha > 1.01f) {
            alpha = 1.0f;
            // unload current scene
            scenes[scene]->unload();
            // load next scene
            scenes[transitionToScene]->init();
            // update global
            scene = transitionToScene;
            transitionFadeOut = true;
        }
    } else {
        alpha -= 0.02f;
        if (alpha < -0.01f) {
            alpha = 0.0f;
            transitionFadeOut = false;
            onTransition = false;
        }
    }
}
template <typename T>
void SceneManager<T>::DrawTransition() {
    // draw "transition" i.e. fade a rectangle the size of the screen
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                  Fade(BLACK, alpha));
}
template <typename T>
void SceneManager<T>::Step() {
    if (!onTransition) {
        scenes[scene]->update();
        if (scenes[scene]->done()) {
            T next = scene_graph[scene];
            if (debug) {
                std::cout << "scene: " << std::to_string(scene).c_str()
                          << " emmitted done" << std::endl;
                std::cout << "transition to: " << std::to_string(next)
                          << std::endl;
            }
            TransitionToScene(next);
        }
    } else {
        UpdateTransition();
    }
    BeginDrawing();
    scenes[scene]->draw();
    if (onTransition) {
        DrawTransition();
    }
    EndDrawing();
}
#endif
