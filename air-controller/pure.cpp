#include "logic.hpp"

bool isHover(Rectangle region) {
    auto pos = GetMousePosition();
    auto X = region.x <= pos.x <= region.x + region.width;
    auto Y = region.y <= pos.y <= region.y + region.height;
    return X && Y;
}
bool isClicked(Rectangle region, int b) {
    return isHover(region) && IsMouseButtonPressed(b);
}
bool complete() {
    // check if the player has completed the flight schedule
    return air_controller_scene.good + air_controller_scene.ok +
               air_controller_scene.bad ==
           (int)air_controller_scene.flight_schedule.size();
}
bool scheduleActive() {
    for (auto &b : air_controller_scene.buttons) {
        if (b.name == "schedule button") {
            return b.active;
        }
    }
    return false;
}
vector<int> arielFlights() {
    vector<int> out{};
    for (auto &flight : flights) {
        if (flight.status == AIR) {
            out.push_back(flight.id);
        }
    }
    return out;
}
vector<pair<int, Rectangle>> arielPositions() {
    vector<pair<int, Rectangle>> out{};
    for (int i = 0; auto &id : arielFlights()) {
        float A = 2 * std::numbers::pi / arielFlights().size();
        auto theta = (air_controller_scene.frame_counter *
                      air_controller_scene.air_rotation_speed) +
                     (A * i);
        auto x = air_controller_scene.air.x +
                 (air_controller_scene.air_radius * cosf(theta));
        auto y = air_controller_scene.air.x +
                 (air_controller_scene.air_radius * sinf(theta));
        out.push_back(make_pair(i, Rectangle{x, y, (float)planeSpriteWidth(),
                                             (float)planeSpriteHeight()}));
        ++i;
    }
    return out;
}
vector<pair<int, Rectangle>> apronPositions() {
    vector<pair<int, Rectangle>> out{};
    for (int i = 0; i < air_controller_scene.apron_count; ++i) {
        auto x = air_controller_scene.apron_position.x + (i * 25);
        auto y = air_controller_scene.apron_position.y - (i * 50);
        auto w = apronSpriteWidth();
        auto h = apronSpriteHeight();
        out.push_back(make_pair(i, Rectangle{x, y, (float)w, (float)h}));
    }
    return out;
}
vector<pair<int, Rectangle>> apronFlagPositions() {
    vector<pair<int, Rectangle>> out{};
    for (auto [id, area] : apronPositions()) {
        area.x -= flagSpriteWidth();
        area.width = flagSpriteWidth();
        area.height = flagSpriteHeight();
        out.push_back(make_pair(id, area));
    }
}
int planeSpriteWidth() { return plane_sprites[0].texture.width; }
int planeSpriteHeight() { return plane_sprites[0].texture.height; }
int apronSpriteWidth() { return 32; }
int apronSpriteHeight() { return 32; }
int flagSpriteWidth() { return 16; }
int flagSpriteHeight() { return 16; }
