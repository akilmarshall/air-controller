/* [Essential]
 * - boolean function constraints for the data, both the
 *   specification/verification of the data model
 * - derived relational variables (required composite tables, etc)
 * - pure functions
 */
#ifndef LOGIC_HXX
#define LOGIC_HXX
#include <cmath>
#include <numbers>
#include <optional>
#include <utility>
#include <vector>

#include "data.hxx"
#include "raylib.h"

using std::make_pair;
using std::make_tuple;
using std::optional;
using std::tuple;
using std::vector;

namespace logic {
namespace constraint {
bool showFlight(data::Flight flight);  // should a flight be shown?
}
namespace derived {
vector<int> arielFlights();
vector<pair<int, Rectangle>> arielPositions();
vector<pair<int, Rectangle>> apronPositions();
vector<pair<int, Rectangle>> apronFlagPositions();
}  // namespace derived
namespace pure {
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
float digitSpriteWidth();
float digitSpriteHeight();

Texture2D backgroundTextureACScene();  // get the current background texture
// for the AC Scene
optional<data::Flight> queryFlight(
    int id);  // attempt to get the data for a flight with id
optional<data::Apron> queryApron(int id);
optional<Texture2D> queryFlightTexture(int id);
optional<Texture2D> queryApronTexture(int id);
optional<Texture2D> queryFlagTexture(int id);
optional<Texture2D> queryDigitTexture(int i);
inline tuple<int, int, int, int> minuteToDigits(int minute) {
    auto h = (int)minute / 60;
    auto m = minute - 60 * h;
    auto a = (int)h / 10;
    auto b = h % 10;
    auto c = (int)m / 10;
    auto d = m % 10;
    return make_tuple(a, b, c, d);
}
optional<data::Apron> queryApronFlightId(
    int flight_id);  // return the apron containing flight_id
}  // namespace pure
}  // namespace logic
#endif
