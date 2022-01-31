#include "control.hpp"

void processUserInput() {
    for (auto &b : air_controller_scene.buttons) {
        if (isClicked(b.region, 0)) {
            b.active = !b.active;
        }
    }
    for (auto &[id, pos] : arielPositions()) {
        if (isClicked(pos, 0)) {
            selectFlight(id);
        }
    }
    for (auto &[id, pos] : apronPositions()) {
        if (isClicked(pos, 0)) {
            selectApron(id);
        }
    }
    for (auto &[id, pos] : apronFlagPositions()) {
        if (isClicked(pos, 0)) {
            selectApron(id);
        }
    }
}
