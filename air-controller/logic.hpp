/* [Essential]
 * - boolean function constraints for the data, both the
 *   specification/verification of the data model
 * - derived relational variables (required composite tables, etc)
 * - pure functions
 */
#ifndef LOGIC_HPP
#define LOGIC_HPP
#include <cmath>
#include <numbers>

#include "data.hpp"
#include "raylib.h"

using std::make_pair;

// contraints
bool showFlight(Flight flight);  // should a flight be shown?
// derived relational data
// pure functions
bool isHover(Rectangle region);
bool isClicked(Rectangle region, int b);
bool complete();  // return true when the player completes the flight schedule
                  // (flights)
bool scheduleActive();
int planeSpriteWidth();
int planeSpriteHeight();
int apronSpriteWidth();
int apronSpriteHeight();
int flagSpriteWidth();
int flagSpriteHeight();

vector<int> arielFlights();
vector<pair<int, Rectangle>> arielPositions();
vector<pair<int, Rectangle>> apronPositions();
vector<pair<int, Rectangle>> apronFlagPositions();
#endif
