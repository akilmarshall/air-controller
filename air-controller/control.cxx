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
#include <iostream>
void control::state::processUserInput() {
    // handle button input
    for (auto &b : data::ACScene::buttons) {
        if (logic::pure::isClicked(b.region, 0)) {
            b.active = !b.active;
        } else if (logic::pure::isHover(b.region)) {
            b.hovered = true;
        } else {
            b.hovered = false;
        }
    }
    // handle input on ariel positions
    for (auto &[id, pos] : logic::derived::arielPositions()) {
        pos.x -= logic::pure::planeTextureWidth() / 2.f;
        pos.y -= logic::pure::planeTextureHeight() / 2.f;
        if (IsMouseButtonPressed(0)) {
            if (logic::pure::isClicked(pos, 0)) {
                api::feeder::setFlightSelect(id);
            } else {
                api::feeder::setFlightSelect({});
            }
        }
        if (logic::pure::isHover(pos)) {
            api::feeder::setFlightHover(id);
        } else {
            api::feeder::setFlightHover({});
        }
    }
    // handle input on the aprons
    for (auto &[id, pos] : logic::derived::apronPositions()) {
        auto apron = logic::pure::queryApron(id);
        if (logic::pure::isClicked(pos, 0)) {
            // TODO, is logic::pure::currentSelectedFlight working?
            // when an apron is selected with a flight already selected that
            // flight and apron need to be  updated
            std::cout << id << "\t" << apron.value().flight_id.value_or(-1)
                      << "\n";
            if (data::ACScene::selected_flight) {
                std::cout << data::ACScene::selected_flight.value()
                          << " flight sent to apron " << apron.value().id
                          << "\n";
                /* api::feeder::setApronFlight(id, selected_flight.value()); */
                /* data::ACScene::selected_flight = id; */
                api::feeder::setFlightSelect(id);
                api::feeder::setFlightStatus(
                    data::ACScene::selected_flight.value(), data::APRON);
            } else {
                std::cout << "no flight selected\n";
            }
            /* api::feeder::selectApron(id); */
            api::feeder::setApronSelect(id);
        }
    }
    // handle input on the apron flags
    for (auto &[id, pos] : logic::derived::apronFlagPositions()) {
        pos.x -= pos.width;
        pos.width *= 2;
        pos.height *= 2;
        /* DrawRectangleLinesEx(pos, 2.f, RED); */
        if (logic::pure::isClicked(pos, 0)) {
            api::feeder::setApronActive(id);
        }
    }
}
