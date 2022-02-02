/* [Essential]
 * abstract data definitions for ALL the essential data
 * this file with logic.hpp form the relational data model for the system
 */
#ifndef DATA_HPP
#define DATA_HPP
#include <array>
#include <list>
#include <map>
#include <optional>
#include <random>
#include <unordered_set>
#include <utility>
#include <vector>

/* #include "api.hpp" */
#include "raylib.h"
#include "scene.hpp"

using std::array;
using std::list;
using std::map;
using std::mt19937;
using std::optional;
using std::pair;
using std::string;
using std::uniform_int_distribution;
using std::unordered_set;
using std::vector;

// Scene information
// I am going to stretch the metaphor but here we go,
// This class represents an inteface to a table of polymorphic scene objects
// that bear at least this common interface. I recognize that currently this
// allows data hiding which is counter to the FRP design principle, further
// analysis required to course correct.
// ...
// maybe its a table of objects with this exact interface but their data is
// public in a table some where else?
// ...
// hold up this is BEHAVIOR, not DATA it shouldn't be here at all
// not in this capacity
//
// struct Scene {
//     // void init();   -> moves to ?
//     // void unload(); -> moves to ?
//     void update();
//     void draw();
//     bool done();
// };
// ...
// instead of using polymorphism for scene management use concepts thus freeing
// the desing of the coupled nature of classes each scene will have it's own
// data namespace instead of using a static class as a namespace
// the init, update, draw, unload, done function bundle will exist in api.hpp
// (other) in some capacity

namespace data {
// The first element of these structs is it's PRIMARY key
// for struct FOO {int id}; if the element FOO_id appears as member of
// another struct it is to be interpreted as a FOREIGN key
// TYPES
// assets
struct Sprite {
    int id;
    Texture2D texture;
};
// game data (dynamic)
enum PlaneStatus { PRE, AIR, APRON, DONE };
struct Flight {
    int id;              // coloquially known as the flight "number"
    int arrival_time;    // time unit is frames
    int departure_time;  // time unit is frames
    int apron_id;        // correct apron for the plane to arrive
    int sprite_id;
    PlaneStatus status;
    bool selected;
    bool hovered;
};
struct Apron {
    // coloquially known as a "Gate"
    int id;                   // coloquially known as the gate "number"
    bool active;              // activated for take off
    optional<int> flight_id;  // optional foreign key
    bool selected;
};
struct Button {
    int id;
    Rectangle region;
    string name;  // contextual name of the button for progamming use
    string text;  // text to write on button
    int fontsize;
    bool active;
    bool hovered;
};
// scenes
namespace ACScene {
bool done;
int frame_counter;
int MIN;            // frames per minute
int hour;           // game clock time
int minute;         // game clock time
int background_id;  // foreign key
Vector2 air;        // location for planes to hold in the air
float air_radius;
float air_rotation_speed;
Vector2 apron_position;
Vector2 clock_position;
Rectangle schedule_area;
Rectangle score_area;
int score_fontsize;
int schedule_fontsize;
Color highlight;
int apron_count;
pair<int, int> flight_number_range;
pair<int, int> arrival_time_minute_range;
pair<int, int> refuel_time_minute_range;
int good;  // flights that left on time and at the proper apron
int ok;    // flights that left on time OR from the proper apron
int bad;   // flights that were late AND from the wrong gate
list<Flight> flight_schedule;
optional<int> selected_flight;
optional<int> selected_apron;
mt19937 gen;
uniform_int_distribution<int> flight_num_gen;
uniform_int_distribution<int> arrival_time_minute_gen;
uniform_int_distribution<int> refuel_time_minute_gen;
uniform_int_distribution<int> apron_id_gen;
uniform_int_distribution<int> plane_texture_gen;
list<Button> buttons;
}  // namespace ACScene
// STATE
// all static objects live here
static array<Sprite, 11> plane_sprites;
static array<Sprite, 10> digit_sprites;
static array<Sprite, 1> apron_sprites;
static array<Sprite, 1> flag_sprites;
static array<Sprite, 1> background_sprites;
static Sprite odot_sprite;
static const int screen_width = 900;
static const int screen_height = 675;
static int fps = 60;

// "tables" of relational data defined here
static list<Flight> flights;  // set of flights the player must manage
static list<Apron> aprons;

// this section FRP to compile to WEB and desktop nicely
// define an instance for each Scene object
enum GameScene { RAYANIM, SPLASH, AIRCONTROLLER, CREDIT };
// define the scene graph
static map<GameScene, GameScene> sceneGraph = {
    {RAYANIM, SPLASH},
    {SPLASH, AIRCONTROLLER},
    {AIRCONTROLLER, CREDIT},
    {CREDIT, AIRCONTROLLER},
};
// initialize the SceneManager
static auto scene_manager = SceneManager<GameScene>(sceneGraph, true);
}  // namespace data
#endif
