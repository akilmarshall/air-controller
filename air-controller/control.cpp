#include "control.hpp"

void control::processUserInput() {
    for (auto &b : data::ACScene::buttons) {
        if (logic::pure::isClicked(b.region, 0)) {
            b.active = !b.active;
        } else if (logic::pure::isHover(b.region)) {
            b.hovered = true;
        } else {
            b.hovered = false;
        }
    }
    for (auto &[id, pos] : logic::pure::arielPositions()) {
        if (logic::pure::isClicked(pos, 0)) {
            api::feeder::selectFlight(id);
        } else if (logic::pure::isHover(pos)) {
        }
    }
    for (auto &[id, pos] : logic::pure::apronPositions()) {
        if (logic::pure::isClicked(pos, 0)) {
            api::feeder::selectApron(id);
        }
    }
    for (auto &[id, pos] : logic::pure::apronFlagPositions()) {
        if (logic::pure::isClicked(pos, 0)) {
            api::feeder::selectApron(id);
        }
    }
}
