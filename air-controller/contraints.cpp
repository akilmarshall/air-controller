#include "logic.hpp"

bool showFlight(Flight flight) {
    switch (flight.status) {
        case PRE: {
            // flights are shown an hour before they arrive
            return flight.arrival_time <=
                   (60 * air_controller_scene.MIN) +
                       air_controller_scene.frame_counter;
        } break;
        case AIR: {
            return true;
        } break;
        case APRON: {
            return true;
        } break;
        case DONE: {
            return false;
        } break;
    }
}
