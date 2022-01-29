#ifndef AIRCONTROLLER_HPP
#define AIRCONTROLLER_HPP
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <numbers>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "raylib.h"
#include "scene.hpp"

using std::optional;
using std::uniform_int_distribution;

struct Plane {
    optional<Vector2> position;
    Texture2D texture;
};
struct Apron {
    // the area at which the air planes are loaded/unloaded, serviced, and
    // boarded
    optional<int> flight_id;  // foreign key
    Vector2 position;
    bool clear{
        false};  // cleared by the ATC for takeoff on the next half hour window
};
struct GUI {
    Rectangle schedule;
    Vector2 schedule_text;
    bool schedule_selected;
    bool schedule_hover;
};
// flights(flight data) stored in a list, generated as a group
struct Flight {
    int flight_num;  // primary key
    /* int refuel_time; */
    int arrival_time;
    int departure_time;  // computed from arrival time + refuel time rounded to
                         // the next hour
    int apron;
    int plane_id;
    bool staged{false};  // landed at an apron
    bool done{false};    // flew away after landing
};
inline std::tuple<int, int, int, int> minuteToDigits(int minute) {
    auto h = (int)minute / 60;
    auto m = minute - 60 * h;
    auto a = (int)h / 10;
    auto b = h % 10;
    auto c = (int)m / 10;
    auto d = m % 10;
    return std::make_tuple(a, b, c, d);
}
class AirControllerScene : public Scene {
   private:
    // game ui data (expect values to be fairly fixed)
    bool done_;
    int frame_counter;
    Texture2D background;
    Vector2 texture_dimension{32.f, 32.f};
    Vector2 center{125.f, 125.f};
    float radius{100.f};
    Color highlight{GOLD};
    float rotation_speed{0.6f};
    int NUMBER_OF_APRONS{4};
    Color hanger_color{GOLD};
    std::array<Texture2D, 10> digit_textures;
    Texture2D odot_texture;
    Texture2D flag_texture;
    std::mt19937 gen;
    uniform_int_distribution<int> flight_num_gen;
    uniform_int_distribution<int> arrival_time_hour_gen;
    uniform_int_distribution<int> arrival_time_minute_gen;
    uniform_int_distribution<int>
        refuel_time_gen;  // how long does the flight refuel (hours)
    uniform_int_distribution<int> apron_gen;
    uniform_int_distribution<int> plane_id_gen{0, 11};

    // game play data (expect values to change)
    int good{0};  // on time, correct gate
    int ok{0};    // on time, or correct gate
    int bad{0};   // late, wrong gate
    std::set<int>
        planes_in_air;  // planes identified by flight_id waiting to land
    std::map<int, Apron> aprons;
    std::map<int, Plane> plane_data;
    optional<int> selected_plane;
    optional<int> selected_apron;
    optional<int> highlight_plane;
    int MINUTE{25};  // frames in one minute
    int minute;
    int hour;
    bool day{true};
    bool over;
    GUI gui;
    std::vector<Flight> flight_schedule;

    optional<Vector2> planeAirPosition(int id);
    std::vector<std::pair<int, Vector2>> planeAirPositions();
    void loadDigitTextures();
    void generateAprons();
    void generatePlanes();
    void updateTime();
    void updateGui(Vector2 position);
    void drawLighting();
    void drawPlanesInAir();
    void drawAprons();
    void drawTime();
    void drawFlightInfo();
    void drawGui();
    void drawSchedule();
    void drawScore();
    void
    processFlight();  // add arriving flights, allow clear flights to take off
    optional<int> flightIDToPlaneID(int id);
    optional<int> getPlane(Vector2 position);
    optional<int> getApron(Vector2 position);
    Flight generateFlight();
    Flight generateFlight(int h);
    Flight generateFlightRelative();  // to the current frame_counter
    Flight& data(int id);             // get flight data for flight_id
    std::vector<Flight> generateSchedule(int n);
    std::vector<Flight> generateScheduleOneShot();  // built in tutorial

   public:
    void init();
    void update();
    void draw();
    void unload();
    bool done();
};
#endif
