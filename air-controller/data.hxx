#ifndef DATA_HXX
#define DATA_HXX
#include <array>
#include <list>
#include <map>
#include <optional>
#include <random>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "raylib.h"

using std::list;
using std::map;
using std::mt19937;
using std::optional;
using std::pair;
using std::string;
using std::uniform_int_distribution;

namespace data {
// product types
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
inline int screen_width = 900;
inline int screen_height = 675;
inline int fps = 60;

// "tables" of relational data defined here
inline list<Flight> flights;  // set of flights the player must manage
inline list<Apron> aprons;

inline map<string, Texture2D> textures;

namespace scene {
enum Scene { RAYANIM, SPLASH, AIRCONTROLLER, CREDIT };
inline Scene current;
inline float alpha;
inline bool transition_fade_out;
inline bool on_transition;
inline Scene from_scene;
inline Scene to_scene;
}  // namespace scene
namespace ACScene {
inline bool done;
inline int frame_counter;
inline int MIN{25};        // frames per minute
inline int hour;           // game clock time
inline int minute;         // game clock time
inline int background_id;  // foreign key
inline Vector2 air;        // location for planes to hold in the air
inline float air_radius;
inline float air_rotation_speed;
inline Vector2 apron_position;
inline Vector2 clock_position;
inline Rectangle schedule_area;
inline Rectangle score_area;
inline int score_fontsize;
inline int schedule_fontsize;
inline Color highlight;
inline int apron_count;
inline pair<int, int> flight_number_range;
inline pair<int, int> arrival_time_minute_range;
inline pair<int, int> refuel_time_minute_range;
inline int good;  // flights that left on time and at the proper apron
inline int ok;    // flights that left on time OR from the proper apron
inline int bad;   // flights that were late AND from the wrong gate
inline list<Flight> flight_schedule;
inline optional<int> selected_flight;
inline optional<int> selected_apron;
inline mt19937 gen;
inline uniform_int_distribution<int> flight_num_gen;
inline uniform_int_distribution<int> arrival_time_minute_gen;
inline uniform_int_distribution<int> refuel_time_minute_gen;
inline uniform_int_distribution<int> apron_id_gen;
inline uniform_int_distribution<int> plane_texture_gen;
inline list<Button> buttons;
}  // namespace ACScene
}  // namespace data
#endif
