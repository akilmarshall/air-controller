/* [Accidental]
 * - gui/api (handle the users input, structured output)
 * - feeders
 * - observers
 */
#ifndef API_HXX
#define API_HXX

#include "control.hxx"
#include "data.hxx"
#include "logic.hxx"
#include "raylib.h"

namespace api {
inline void drawBackground() {
    DrawTexture(control::state::getBG(), 0, 0, WHITE);
}
namespace scene {
void init(data::scene::Scene scene);  // define and initialize the initial scene
void transition(
    data::scene::Scene next);  // transition from currentscene to scene
void updateTransition();
void drawTransition();
void step();  // void function allowing control to be passed off to a web
              // engine, central thread of control for the game
              // engine/definition of the scene graph
void ACSceneInit();
void ACSceneUpdate();
void ACSceneDraw();
void ACSceneUnload();
bool ACSceneDone();
}  // namespace scene
namespace feeder {
void generateFlightSetA();
void updateTime();
void updateFlights();  // flight data is updated here
void selectFlight(int id);
void selectApron(int apron_id);
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