#include "api.hpp"

// api
void step() { scene_manager.Step(); }
// feeders
void generateFlightSetA() {
    auto flight = Flight{.id = 1,
                         .arrival_time = 75 * air_controller_scene.MIN,
                         .departure_time = 100 * air_controller_scene.MIN,
                         .apron_id = 1,
                         .status = PRE,
                         .selected = false};
    flights.push_back(flight);
}
void updateTime() {
    auto x =
        (air_controller_scene.frame_counter + 1) % air_controller_scene.MIN;
    if (x == 1) {
        air_controller_scene.minute += 1;
        if (air_controller_scene.minute == 60) {
            air_controller_scene.minute = 0;
            air_controller_scene.hour += 1;
            if (air_controller_scene.hour > 23) {
                air_controller_scene.hour = 0;
            }
        }
    }
}
void updateFlights() {
    for (auto &flight : flights) {
        switch (flight.status) {
            case PRE: {
                if (showFlight(flight)) {
                    flight.status = AIR;
                }
            } break;
            case AIR: {
            } break;
            case APRON: {
            } break;
            case DONE: {
            } break;
        }
    }
}
void selectFlight(int id) {
    for (auto &flight : flights) {
        flight.selected = (flight.id == id);
    }
}
// observers
