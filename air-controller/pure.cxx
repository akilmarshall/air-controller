#include "logic.hxx"

bool logic::pure::isHover(Rectangle region) {
    auto pos = GetMousePosition();
    auto X = region.x <= pos.x && pos.x <= region.x + region.width;
    auto Y = region.y <= pos.y && pos.y <= region.y + region.height;
    return X && Y;
}
bool logic::pure::isClicked(Rectangle region, int b) {
    return isHover(region) && IsMouseButtonPressed(b);
}
bool logic::pure::complete() {
    // check if the player has completed the flight schedule
    return data::ACScene::good + data::ACScene::ok + data::ACScene::bad ==
           (int)data::flights.size();
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
        out.push_back(make_pair(
            id, Rectangle{x, y, (float)logic::pure::planeTextureWidth(),
                          (float)logic::pure::planeTextureHeight()}));
        ++i;
    }
    return out;
}
vector<pair<int, Rectangle>> logic::derived::apronPositions() {
    vector<pair<int, Rectangle>> out{};
    for (int i = 0; i < data::ACScene::apron_count; ++i) {
        float x = data::ACScene::apron_position.x + (i * 25);
        float y = data::ACScene::apron_position.y - (i * 50);
        float w = logic::pure::apronTextureWidth();
        float h = logic::pure::apronTextureHeight();
        out.push_back(make_pair(i, Rectangle{x, y, w, h}));
    }
    return out;
}
vector<pair<int, Rectangle>> logic::derived::apronFlagPositions() {
    vector<pair<int, Rectangle>> out{};
    for (auto [id, area] : apronPositions()) {
        area.x -= logic::pure::flagTextureWidth();
        area.width = logic::pure::flagTextureWidth();
        area.height = logic::pure::flagTextureHeight();
        out.push_back(make_pair(id, area));
    }
    return out;
}
int logic::pure::planeTextureWidth() { return 32; }
int logic::pure::planeTextureHeight() { return 32; }
int logic::pure::apronTextureWidth() { return 32; }
int logic::pure::apronTextureHeight() { return 32; }
int logic::pure::flagTextureWidth() { return 16; }
int logic::pure::flagTextureHeight() { return 16; }
float logic::pure::digitTextureWidth() { return 16; }
float logic::pure::digitTextureHeight() { return 16; }

Texture2D logic::pure::backgroundTextureACScene() {
    return data::textures["ito"];
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
    auto s = TextFormat("plane%i", id);
    if (data::textures.contains(s)) {
        return data::textures[s];
    }
    return {};
}
optional<Texture2D> logic::pure::queryApronTexture(int id) {
    if (data::textures.contains(TextFormat("apron%i", id))) {
        return data::textures[TextFormat("apron%i", id)];
    }
    return {};
}
optional<Texture2D> logic::pure::queryFlagTexture(int id) {
    if (data::textures.contains("flag")) {
        return data::textures["flag"];
    }
    return {};
}
optional<Texture2D> logic::pure::queryDigitTexture(int i) {
    if (data::textures.contains(TextFormat("digit%i", i))) {
        return data::textures[TextFormat("digit%i", i)];
    }
    return {};
}
