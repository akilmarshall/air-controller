#include "logic.hpp"

bool showFlight(data::Flight flight) {
    switch (flight.status) {
        case data::PRE: {
            // flights are shown an hour before they arrive
            return flight.arrival_time <=
                   (60 * data::ACScene::MIN) + data::ACScene::frame_counter;
        } break;
        case data::AIR: {
            return true;
        } break;
        case data::APRON: {
            return true;
        } break;
        case data::DONE: {
            return false;
        } break;
    }
}
