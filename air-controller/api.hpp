/* [Accidental]
 * - gui (handle the users input)
 * - feeders
 * - observers
 */
#ifndef GUI_HPP
#define GUI_HPP
#include "data.hpp"
#include "logic.hpp"
// gui / api
// api
void step();  // void function allowing control to be passed off to a web engine
// feeders
void generateFlightSetA();
void updateTime();
void updateFlights();  // flight data is updated here
void selectFlight(int id);
void selectApron(int id);
// observers
#endif
