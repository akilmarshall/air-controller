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

#include "raylib.h"

using std::array;
using std::list;
using std::mt19937;
using std::optional;
using std::pair;
using std::string;
using std::uniform_int_distribution;

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
static bool done;
static int frame_counter;
static int MIN;            // frames per minute
static int hour;           // game clock time
static int minute;         // game clock time
static int background_id;  // foreign key
static Vector2 air;        // location for planes to hold in the air
static float air_radius;
static float air_rotation_speed;
static Vector2 apron_position;
static Vector2 clock_position;
static Rectangle schedule_area;
static Rectangle score_area;
static int score_fontsize;
static int schedule_fontsize;
static Color highlight;
static int apron_count;
static pair<int, int> flight_number_range;
static pair<int, int> arrival_time_minute_range;
static pair<int, int> refuel_time_minute_range;
static int good;  // flights that left on time and at the proper apron
static int ok;    // flights that left on time OR from the proper apron
static int bad;   // flights that were late AND from the wrong gate
static list<Flight> flight_schedule;
static optional<int> selected_flight;
static optional<int> selected_apron;
static mt19937 gen;
static uniform_int_distribution<int> flight_num_gen;
static uniform_int_distribution<int> arrival_time_minute_gen;
static uniform_int_distribution<int> refuel_time_minute_gen;
static uniform_int_distribution<int> apron_id_gen;
static uniform_int_distribution<int> plane_texture_gen;
static list<Button> buttons;
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
namespace scene {
enum GameScene { RAYANIM, SPLASH, AIRCONTROLLER, CREDIT };
static GameScene current_scene;
static float alpha;
static bool transition_fade_out;
static bool on_transition;
static GameScene from_scene;
static GameScene to_scene;
}  // namespace scene
}  // namespace data
#endif
