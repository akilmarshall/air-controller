#include "control.hxx"

#include "api.hxx"

void control::state::loadTextures() {
    // background textures
    data::textures["ito"] = LoadTexture("resources/ito.png");
    // plane textures
    for (int i = 0; i < 12; ++i) {
        data::textures[TextFormat("plane%i", i)] =
            LoadTexture(TextFormat("resources/ship_%i.png", i));
    }
    // digit textures
    for (int i = 0; i < 10; ++i) {
        data::textures[TextFormat("digit%i", i)] =
            LoadTexture(TextFormat("resources/digit_%i.png", i));
    }
    data::textures["odot"] = LoadTexture("resources/odot.png");
    // flag texture
    data::textures["flag"] = LoadTexture("resources/flag.png");
}
void control::state::processUserInput() {
    for (auto &b : data::ACScene::buttons) {
        if (logic::pure::isClicked(b.region, 0)) {
            b.active = !b.active;
        } else if (logic::pure::isHover(b.region)) {
            b.hovered = true;
        } else {
            b.hovered = false;
        }
    }
    for (auto &[id, pos] : logic::derived::arielPositions()) {
        if (logic::pure::isClicked(pos, 0)) {
            api::feeder::selectFlight(id);
        } else if (logic::pure::isHover(pos)) {
        }
    }
    for (auto &[id, pos] : logic::derived::apronPositions()) {
        if (logic::pure::isClicked(pos, 0)) {
            api::feeder::selectApron(id);
        }
    }
    for (auto &[id, pos] : logic::derived::apronFlagPositions()) {
        if (logic::pure::isClicked(pos, 0)) {
            api::feeder::selectApron(id);
        }
    }
}
