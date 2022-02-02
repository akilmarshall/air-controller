/* [Accidental]
 * - gui (handle the users input)
 * - feeders
 * - observers
 */
#ifndef GUI_HPP
#define GUI_HPP
#include "data.hpp"
#include "logic.hpp"
#include "raylib.h"

namespace api {
// gui / api
// api
void step();  // void function allowing control to be passed off to a web engine

void ACSceneInit();
void ACSceneUpdate();
void ACSceneDraw();
void ACSceneUnload();
bool ACSceneDone();

namespace feeder {
void generateFlightSetA();
void updateTime();
void updateFlights();  // flight data is updated here
void selectFlight(int id);
void selectApron(int id);
}  // namespace feeder

namespace observer {
void drawBackground();
void drawButtons();
void drawFlights();
void drawAprons();
void drawClock();
void drawScore();
void drawSchedule();
}  // namespace observer

}  // namespace api
#endif
