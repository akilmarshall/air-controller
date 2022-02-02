#include "logic.hpp"

bool logic::pure::isHover(Rectangle region) {
    auto pos = GetMousePosition();
    auto X = region.x <= pos.x <= region.x + region.width;
    auto Y = region.y <= pos.y <= region.y + region.height;
    return X && Y;
}
bool logic::pure::isClicked(Rectangle region, int b) {
    return isHover(region) && IsMouseButtonPressed(b);
}
bool logic::pure::complete() {
    // check if the player has completed the flight schedule
    return data::ACScene::good + data::ACScene::ok + data::ACScene::bad ==
           (int)data::ACScene::flight_schedule.size();
}
bool logic::pure::scheduleActive() {
    for (auto &b : data::ACScene::buttons) {
        if (b.name == "schedule button") {
            return b.active;
        }
    }
    return false;
}
vector<int> logic::derived::arielFlights() {
    vector<int> out{};
    for (auto &flight : data::flights) {
        if (flight.status == data::AIR) {
            out.push_back(flight.id);
        }
    }
    return out;
}
vector<pair<int, Rectangle>> logic::derived::arielPositions() {
    vector<pair<int, Rectangle>> out{};
    for (int i = 0; auto &id : arielFlights()) {
        float A = 2 * std::numbers::pi / arielFlights().size();
        auto theta =
            (data::ACScene::frame_counter * data::ACScene::air_rotation_speed) +
            (A * i);
        auto x =
            data::ACScene::air.x + (data::ACScene::air_radius * cosf(theta));
        auto y =
            data::ACScene::air.y + (data::ACScene::air_radius * sinf(theta));
        out.push_back(
            make_pair(i, Rectangle{x, y, (float)logic::pure::planeSpriteWidth(),
                                   (float)logic::pure::planeSpriteHeight()}));
        ++i;
    }
    return out;
}
vector<pair<int, Rectangle>> logic::derived::apronPositions() {
    vector<pair<int, Rectangle>> out{};
    for (int i = 0; i < data::ACScene::apron_count; ++i) {
        auto x = data::ACScene::apron_position.x + (i * 25);
        auto y = data::ACScene::apron_position.y - (i * 50);
        auto w = logic::pure::apronSpriteWidth();
        auto h = logic::pure::apronSpriteHeight();
        out.push_back(make_pair(i, Rectangle{x, y, (float)w, (float)h}));
    }
    return out;
}
vector<pair<int, Rectangle>> logic::derived::apronFlagPositions() {
    vector<pair<int, Rectangle>> out{};
    for (auto [id, area] : apronPositions()) {
        area.x -= logic::pure::flagSpriteWidth();
        area.width = logic::pure::flagSpriteWidth();
        area.height = logic::pure::flagSpriteHeight();
        out.push_back(make_pair(id, area));
    }
}
int planeSpriteWidth() { return data::plane_sprites[0].texture.width; }
int planeSpriteHeight() { return data::plane_sprites[0].texture.height; }
int apronSpriteWidth() { return 32; }
int apronSpriteHeight() { return 32; }
int flagSpriteWidth() { return 16; }
int flagSpriteHeight() { return 16; }
float digitSpriteWidth() { return data::digit_sprites[0].texture.width; }
float digitSpriteHeight() { return data::digit_sprites[0].texture.height; }

data::Sprite logic::pure::backgroundSpriteACScene() {
    return data::background_sprites[data::ACScene::background_id];
}
optional<data::Flight> logic::pure::queryFlight(int id) {
    for (auto &flight : data::flights) {
        if (flight.id == id) {
            return flight;
        }
    }
    return {};
}
optional<data::Apron> logic::pure::queryApron(int id) {
    for (auto &apron : data::aprons) {
        if (apron.id == id) {
            return apron;
        }
    }
    return {};
}
optional<data::Apron> logic::pure::queryApronFlightId(int flight_id) {
    for (auto &apron : data::aprons) {
        if (apron.flight_id) {
            if (apron.flight_id == flight_id) {
                return apron;
            }
        }
    }
    return {};
}
optional<Texture2D> logic::pure::queryFlightTexture(int id) {
    for (auto &flight : data::flights) {
        if (flight.id == id) {
            // feels weird to explicitly use [] here instead of an abstracted
            // method call
            auto id = flight.sprite_id;
            return data::plane_sprites[id];
        }
    }
    return {};
}
optional<data::Sprite> logic::pure::queryFlightSprite(int id) {
    for (auto &plane_sprite : data::plane_sprites) {
        if (plane_sprite.id == id) {
            return plane_sprite;
        }
    }
    return {};
}
optional<data::Sprite> logic::pure::queryApronSprite(int id) {
    for (auto &apron_sprite : data::apron_sprites) {
        if (apron_sprite.id == id) {
            return apron_sprite;
        }
    }
    return {};
}
optional<data::Sprite> logic::pure::queryFlagSprite(int id) {
    return data::flag_sprites[0];  // "hack" this method is overbuilt, there is
                                   // only 1 texture, in the future no more of
                                   // this
    for (auto &flag_sprite : data::flag_sprites) {
        if (flag_sprite.id == id) {
            return flag_sprite;
        }
    }
    return {};
}
optional<data::Sprite> logic::pure::queryDigitSprite(int i) {
    if (0 <= i && i <= 9) {
        return data::digit_sprites[i];
    }
    return {};
}
