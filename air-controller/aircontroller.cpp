#include "api.hpp"
#include "control.hpp"
#include "data.hpp"
/* #include "aircontroller.hpp" */

/*
// AirControllerScene implementation
// private
void AirControllerScene::drawLighting() {
    Color c;
    if (day) {
        c = GOLD;
    } else {
        c = BLUE;
    }
    DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), c, WHITE);
}
void AirControllerScene::generatePlanes() {
    // load the textures of all of the plane types
    for (int i = 0; i < 12; ++i) {
        auto name = TextFormat("resources/ship_%i.png", i);
        optional<Vector2> pos = planeAirPosition(i);
        plane_data[i] = Plane{pos, LoadTexture(name)};
    }
}
void AirControllerScene::updateTime() {
    auto x = (frame_counter + 1) % MINUTE;
    if (x == 1) {
        minute += 1;
        if (minute == 60) {
            minute = 0;
            hour += 1;
            if (hour > 23) {
                hour = 0;
                day = !day;
            }
        }
    }
}
void AirControllerScene::updateGui(Vector2 position) {
    auto Sx = gui.schedule.x <= position.x &&
              position.x <= gui.schedule.x + gui.schedule.width;
    auto Sy = gui.schedule.y <= position.y &&
              position.y <= gui.schedule.y + gui.schedule.height;

    if (IsMouseButtonPressed(0)) {
        // toggle selected states
        if (Sx && Sy) {
            gui.schedule_selected = !gui.schedule_selected;
        }
        if (selected_plane && !selected_apron) {
            selected_apron = getApron(position);
            if (selected_apron && !aprons[selected_apron.value()].flight_id) {
                // assign selected_plane to the apron data
                aprons[selected_apron.value()].flight_id = selected_plane;
                planes_in_air.erase(selected_plane.value());
                auto itr =
                    find_if(flight_schedule.begin(), flight_schedule.end(),
                            [&](Flight f) {
                                return f.flight_num == selected_plane.value();
                            });
                itr->staged = true;
                selected_apron = {};
                selected_plane = {};
            }
        }
        selected_plane = getPlane(position);
        selected_apron = getApron(position);

    } else {
        // do exclusive hover stuff
        gui.schedule_hover = Sx && Sy;
    }
    for (auto &[id, apron] : aprons) {
        auto &[flight_id, pos, clear] = apron;
        auto X = pos.x - 28 <= position.x && position.x <= pos.x - 12;
        auto Y = pos.y + 5 <= position.y && position.y <= pos.y + 21;
        if (X && Y && IsMouseButtonPressed(0)) {
            clear = !clear;
        }
        if ((frame_counter + 1) % (30 * MINUTE) == 0) {
            if (clear && flight_id) {
                auto t = data(flight_id.value());
                auto &[num, a, d, apron, plane, staged, done] = t;
                // keep stats
                auto timely = frame_counter <= d;
                auto correct_gate = apron == id;
                if (timely && correct_gate) {
                    ++good;
                } else if (correct_gate || timely) {
                    ++ok;
                } else {
                    ++bad;
                }
                auto itr = flight_schedule.begin();
                while (itr != flight_schedule.end()) {
                    if (itr->flight_num == flight_id) {
                        break;
                    }
                    ++itr;
                }
                flight_schedule.erase(itr);
                done = true;
                flight_id = {};
            }
            clear = false;
        }
    }
}
void AirControllerScene::generateAprons() {
    // generate the apron data to be used over the life spawn of the Scene
    aprons.clear();
    auto p = Vector2{570.f, 420.f};  // location of the first apron
    float delta_x = 25;
    float delta_y = -50;
    for (int i = 0; i < NUMBER_OF_APRONS; ++i) {
        // aprons have a position but no plane by default
        auto apron = Apron{{}, p};
        aprons[i] = apron;
        // increment the apron location
        p.x += delta_x;
        p.y += delta_y;
    }
}
optional<Vector2> AirControllerScene::planeAirPosition(int id) {
    // iterator to locate the plane air position for flight id
    auto f = std::find(planes_in_air.begin(), planes_in_air.end(), id);
    if (f != planes_in_air.end()) {
        // slow down time by a magic number
        float t = frame_counter * 0.01f;
        // compute the index for the plane id
        auto i = std::distance(planes_in_air.begin(), f);
        float A = 2 * std::numbers::pi / planes_in_air.size();
        auto x = center.x + (radius * cosf((t * rotation_speed) + (A * i)));
        auto y = center.y + (radius * sinf((t * rotation_speed) + (A * i)));
        return Vector2{x, y};
    }
    return {};
}
std::vector<std::pair<int, Vector2>> AirControllerScene::planeAirPositions() {
    // use planeAirPosition for each element in planes_in_air and return a
    // vector containing the position of each aerial plane and it's flight id
    std::vector<std::pair<int, Vector2>> out = {};
    for (auto &id : planes_in_air) {
        auto pos = planeAirPosition(id);
        if (pos) {
            out.push_back(std::make_pair(id, pos.value()));
        }
    }
    return out;
}
void AirControllerScene::drawPlanesInAir() {
    for (auto &[flight_id, pos] : planeAirPositions()) {
        optional<int> plane_id = flightIDToPlaneID(flight_id);
        Texture2D texture;
        if (plane_id) {
            texture = plane_data[plane_id.value()].texture;
        }
        // condition A, check if this plane is the selected plane
        // condition B, check if this plane is the highlighted plane
        auto A = selected_plane && selected_plane.value() == flight_id;
        auto B = highlight_plane && highlight_plane.value() == flight_id;
        if (A || B) {
            // DrawTexture(texture, pos.x, pos.y, WHITE);
            Rectangle source{0.f, 0.f, 32.f, 32.f};
            Rectangle dest{pos.x - 6, pos.y - 6, 50, 50};
            DrawTexturePro(texture, source, dest, Vector2{8.f, 8.f}, 0.f,
                           WHITE);
            // DrawTexture(texture, pos.x, pos.y, highlight);
        } else {
            DrawTexture(texture, pos.x, pos.y, WHITE);
        }
    }
}
optional<int> AirControllerScene::flightIDToPlaneID(int id) {
    for (auto &f : flight_schedule) {
        if (f.flight_num == id) {
            return f.plane_id;
        }
    }
    return {};
}
void AirControllerScene::drawAprons() {
    for (auto &[apron_id, apron] : aprons) {
        auto &[flight_id, position, clear] = apron;
        if (flight_id) {
            auto texture =
                plane_data[flightIDToPlaneID(flight_id.value()).value()]
                    .texture;
            DrawTexture(texture, apron.position.x, apron.position.y, WHITE);
        }
        DrawRectangleLines(position.x, position.y, texture_dimension.x,
                           texture_dimension.y, hanger_color);
        auto text = TextFormat("Gate %i", 1 + apron_id);
        auto width = MeasureText(text, 20);
        DrawRectangle(position.x + 40, position.y, width + 20, 30,
                      Fade(DARKGRAY, 0.6));
        DrawText(text, position.x + 50, position.y + 5, 20, hanger_color);

        Color c;
        if (clear) {
            c = WHITE;
        } else {
            c = DARKPURPLE;
        }
        DrawTexture(flag_texture, position.x - 25, position.y + 4, c);
        // DrawRectangleLines(position.x - 28, position.y + 5, 20, 20, RED);
    }
}
void AirControllerScene::drawTime() {
    // ab:cd
    auto x = GetScreenWidth() / 2.f;
    auto y = GetScreenHeight() / 8.f;
    int a = hour / 10;
    int b = hour % 10;
    int c = minute / 10;
    int d = minute % 10;
    auto source = Rectangle{0.f, 0.f, 16.f, 16.f};
    auto dest_a = Rectangle{x - 64, y, 32.f, 32.f};
    DrawTexturePro(digit_textures[a], source, dest_a, Vector2{8.f, 8.f}, 0.f,
                   WHITE);
    auto dest_b = Rectangle{x - 32, y, 32.f, 32.f};
    DrawTexturePro(digit_textures[b], source, dest_b, Vector2{8.f, 8.f}, 0.f,
                   WHITE);
    source = Rectangle{0.f, 0.f, 16.f, 16.f};
    Rectangle dest{x, y, 32, 32};
    DrawTexturePro(odot_texture, source, dest, Vector2{8.f, 8.f}, 0.f, WHITE);
    dest.y -= 12;
    DrawTexturePro(odot_texture, source, dest, Vector2{8.f, 8.f}, 0.f / 2,
                   WHITE);
    auto dest_c = Rectangle{x + 32, y, 32.f, 32.f};
    DrawTexturePro(digit_textures[c], source, dest_c, Vector2{8.f, 8.f}, 0.f,
                   WHITE);
    auto dest_d = Rectangle{x + 64, y, 32.f, 32.f};
    DrawTexturePro(digit_textures[d], source, dest_d, Vector2{8.f, 8.f}, 0.f,
                   WHITE);
}
void AirControllerScene::drawFlightInfo() {
    std::string s;
    if (selected_plane) {
        s = std::to_string(selected_plane.value());
    } else {
        s = std::string("none");
    }
    auto f = 20;
    if (selected_plane) {
        DrawText("Flight#", center.x - 50, center.y - 40, f, BLACK);
        DrawText(TextFormat("%i", selected_plane.value()), center.x + 25,
                 center.y - 40, f, BLACK);

        DrawText("Arrive", center.x - 50, center.y - 12.5, f, BLACK);
        DrawText("Depart", center.x - 50, center.y + 15, f, BLACK);
        DrawText("Gate", center.x - 30, center.y + 42.5, f, BLACK);
        for (auto &flight : flight_schedule) {
            if (selected_plane.value() == flight.flight_num) {
                auto t = minuteToDigits(flight.arrival_time / MINUTE);
                auto &[a, b, c, d] = t;
                DrawText(TextFormat("%i%i:%i%i", a, b, c, d), center.x + 25,
                         center.y - 12.5, f, BLACK);
                t = minuteToDigits(flight.departure_time / MINUTE);
                auto &[v, w, h, k] = t;
                DrawText(TextFormat("%i%i:%i%i", v, w, h, k), center.x + 25,
                         center.y + 15, f, BLACK);
                DrawText(TextFormat("%i", flight.apron + 1), center.x + 25,
                         center.y + 42.5, f, BLACK);
                break;
            }
        }
    }
}
void AirControllerScene::drawGui() {
    // draw schedule button
    Color c;
    if (gui.schedule_selected) {
        // if arrival selected
        c = LIGHTGRAY;
    } else if (gui.schedule_hover) {
        // if arrival is hovered
        c = GRAY;
    } else {
        // default state
        c = DARKGRAY;
    }
    DrawRectangleRounded(gui.schedule, 0.4, 100, c);
    auto r = Rectangle{gui.schedule};
    r.x -= 2;
    r.y -= 2;
    r.width += 4;
    r.height += 4;
    DrawRectangleRounded(r, 0.4, 100, Fade(DARKGRAY, 0.6f));
    DrawText("Schedule", gui.schedule_text.x, gui.schedule_text.y, 30, GOLD);
}
void AirControllerScene::drawSchedule() {
    auto x = GetScreenWidth() / 2.f;
    auto y = GetScreenHeight() / 2.f;
    auto shadow = Rectangle{50.f, y - 45, x, y + 5};
    if (gui.schedule_selected) {
        auto mousePos = GetMousePosition();
        DrawRectangleRounded(shadow, 0.1, 100, Fade(DARKGRAY, 0.8f));
        DrawText("Flight #", 60.f, y - 40, 20, GOLD);
        DrawText("Arrival", 190.f, y - 40, 20, GOLD);
        DrawText("Departure", 310.f, y - 40, 20, GOLD);
        //  DrawRectangleLines(450.f, y - 40, 16, 16, RED);   // sprite
        // position
        DrawRectangle(60.f, y - 15, x - 20, 4, GOLD);
        for (int i = 1; auto &flight : flight_schedule) {
            auto &[num, arrival, departure, apron, texture_id, staged, done] =
                flight;
            // write Flight #
            DrawText(std::to_string(num).c_str(), 60.f, (y - 30) + i * 25, 20,
                     GOLD);
            // decompose into digits
            auto t = minuteToDigits(arrival / MINUTE);
            auto &[a, b, c, d] = t;
            // write Arrival
            DrawText(TextFormat("%i%i:%i%i", a, b, c, d), 190.f,
                     (y - 30) + i * 25, 20, GOLD);
            t = minuteToDigits(departure / MINUTE);
            auto &[v, w, h, k] = t;
            // write Departure
            // decompose into digits
            DrawText(TextFormat("%i%i:%i%i", v, w, h, k), 310.f,
                     (y - 30) + i * 25, 20, GOLD);
            // write Texture
            auto texture = plane_data[texture_id].texture;
            if (staged) {
                DrawText(TextFormat("G%i", apron + 1), 450.f, (y - 27) + i * 25,
                         20, GOLD);
            } else if (!done) {
                DrawTexture(texture, 450.f, (y - 35) + i * 25, WHITE);
            }
            // optional highlight
            //
            auto X = 50.f <= mousePos.x && mousePos.x <= x + 50;
            auto y_ = (y - 32) + i * 25;
            auto Y = y_ <= mousePos.y && mousePos.y <= y_ + 25;
            if (X && Y) {
                DrawRectangleLines(50.f, (y - 32) + i * 25, x, 25, GOLD);
            }
            ++i;
            if (i > 12) break;
        }
    }
}
void AirControllerScene::drawScore() {
    DrawRectangle(GetScreenWidth() / 4, GetScreenHeight() / 4,
                  GetScreenWidth() / 2, GetScreenHeight() / 2, DARKGRAY);

    auto x = 290.f;
    auto y = 250.f;
    DrawText(TextFormat("Good Departures: %i", good), x, y, 30, GOLD);
    DrawText(TextFormat("Ok Departures: %i", ok), x, y + 50, 30, GOLD);
    DrawText(TextFormat("Bad Departures: %i", bad), x, y + 100, 30, GOLD);
}
void AirControllerScene::processFlight() {
    for (auto &flight : flight_schedule) {
        auto &[num, a, d, apron, plane, staged, done] = flight;
        if (a <= (60 * MINUTE) + frame_counter && !staged) {
            planes_in_air.insert(num);
        }
    }
    // win threashold
    if (good + ok + bad > (int)flight_schedule.size() - 1) {
        over = true;
        if (IsMouseButtonPressed(0)) {
            done_ = true;
        }
    }
}
optional<int> AirControllerScene::getPlane(Vector2 position) {
    float s = 0.6f;
    auto w = s * texture_dimension.x;
    auto h = s * texture_dimension.y;
    for (auto &[flight_id, p] : planeAirPositions()) {
        auto X = p.x - w <= position.x && position.x <= p.x + w;
        auto Y = p.y - h <= position.y && position.y <= p.y + h;
        if (X && Y) {
            return flight_id;
        }
    }
    return {};
}
optional<int> AirControllerScene::getApron(Vector2 position) {
    auto w = 36;
    auto h = 36;
    for (auto &[apron_id, apron] : aprons) {
        auto [_, p, clear] = apron;
        auto X = p.x <= position.x && position.x <= p.x + w;
        auto Y = p.y <= position.y && position.y <= p.y + h;
        if (X && Y) {
            return apron_id;
        }
    }
    return {};
}
void AirControllerScene::loadDigitTextures() {
    for (int i = 0; auto &texture : digit_textures) {
        texture = LoadTexture(TextFormat("resources/digit_%i.png", i));
        ++i;
    }
    odot_texture = LoadTexture("resources/odot.png");
}
Flight AirControllerScene::generateFlight() {
    auto flight_num = flight_num_gen(gen);
    auto arrival_time_hour = arrival_time_hour_gen(gen);
    auto arrival_time_minute = arrival_time_minute_gen(gen);
    auto refuel_time = refuel_time_gen(gen);
    auto apron = apron_gen(gen);
    auto plane_id = plane_id_gen(gen);

    auto a_time_min = (arrival_time_hour * 60) + arrival_time_minute;
    auto d = a_time_min + refuel_time;  // d the time in minutes to depart
    int i;
    for (i = 0; i < 60; ++i) {
        if ((d + i) % 60 == 0) {
            break;
        }
    }
    d += i;
    return Flight{
        .flight_num = flight_num,
        .arrival_time = a_time_min * MINUTE,
        .departure_time = d * MINUTE,
        .apron = apron,
        .plane_id = plane_id,
    };
}
Flight AirControllerScene::generateFlight(int h) {
    auto flight_num = flight_num_gen(gen);
    auto arrival_time_minute = arrival_time_minute_gen(gen);
    auto refuel_time = refuel_time_gen(gen);
    auto apron = apron_gen(gen);
    auto plane_id = plane_id_gen(gen);

    auto a_time_min = (h * 60) + arrival_time_minute;
    auto d = a_time_min + refuel_time;  // d the time in minutes to depart
    int i;
    for (i = 0; i < 60; ++i) {
        if ((d + i) % 60 == 0) {
            break;
        }
    }
    d += i;
    return Flight{
        .flight_num = flight_num,
        .arrival_time = a_time_min * MINUTE,
        .departure_time = d * MINUTE,
        .apron = apron,
        .plane_id = plane_id,
    };
}
Flight &AirControllerScene::data(int id) {
    optional<Flight> out{};
    for (auto &flight : flight_schedule) {
        if (flight.flight_num == id) {
            return flight;
        }
    }
    return out.value();
}
std::vector<Flight> AirControllerScene::generateSchedule(int n) {
    std::vector<Flight> schedule{};
    while ((int)schedule.size() < n) {
        auto flight = generateFlight();
        schedule.push_back(flight);
    }
    return schedule;
}
std::vector<Flight> AirControllerScene::generateScheduleOneShot() {
    std::vector<Flight> schedule{};
    schedule.push_back(
        Flight{1, 15 * MINUTE, 60 * MINUTE, 1, plane_id_gen(gen)});
    schedule.push_back(
        Flight{2, 90 * MINUTE, 150 * MINUTE, 2, plane_id_gen(gen)});
    schedule.push_back(
        Flight{3, 100 * MINUTE, 150 * MINUTE, 2, plane_id_gen(gen)});
    schedule.push_back(
        Flight{4, 110 * MINUTE, 175 * MINUTE, 2, plane_id_gen(gen)});
    schedule.push_back(generateFlight(3));
    schedule.push_back(generateFlight(3));
    schedule.push_back(generateFlight(4));
    schedule.push_back(generateFlight(4));
    schedule.push_back(generateFlight(4));
    schedule.push_back(generateFlight(5));
    schedule.push_back(generateFlight(5));
    schedule.push_back(generateFlight(5));
    schedule.push_back(generateFlight(6));
    schedule.push_back(generateFlight(6));
    schedule.push_back(generateFlight(6));
    schedule.push_back(generateFlight(6));
    schedule.push_back(generateFlight(7));
    schedule.push_back(generateFlight(7));
    schedule.push_back(generateFlight(7));
    schedule.push_back(generateFlight(7));
    schedule.push_back(generateFlight(8));
    schedule.push_back(generateFlight(8));
    schedule.push_back(generateFlight(8));
    schedule.push_back(generateFlight(8));
    schedule.push_back(generateFlight(8));
    schedule.push_back(generateFlight(9));
    schedule.push_back(generateFlight(9));
    schedule.push_back(generateFlight(9));
    schedule.push_back(generateFlight(9));
    schedule.push_back(generateFlight(9));
    schedule.push_back(generateFlight(10));
    schedule.push_back(generateFlight(10));
    schedule.push_back(generateFlight(10));
    schedule.push_back(generateFlight(10));
    schedule.push_back(generateFlight(10));
    schedule.push_back(generateFlight(10));
    schedule.push_back(generateFlight(11));
    schedule.push_back(generateFlight(11));
    schedule.push_back(generateFlight(11));
    schedule.push_back(generateFlight(11));
    schedule.push_back(generateFlight(11));
    schedule.push_back(generateFlight(11));
    schedule.push_back(generateFlight(12));
    schedule.push_back(generateFlight(12));
    schedule.push_back(generateFlight(12));
    schedule.push_back(generateFlight(12));
    schedule.push_back(generateFlight(12));
    schedule.push_back(generateFlight(12));
    schedule.push_back(generateFlight(12));
    schedule.push_back(generateFlight(13));
    schedule.push_back(generateFlight(13));
    schedule.push_back(generateFlight(13));
    schedule.push_back(generateFlight(13));
    schedule.push_back(generateFlight(13));
    schedule.push_back(generateFlight(13));
    schedule.push_back(generateFlight(13));

    return schedule;
}
*/
void AirControllerScene::init() {
    done_ = false;
    frame_counter = 0;
    minute = 0;
    hour = 0;
    good = 0;
    ok = 0;
    bad = 0;
    background_id = 0;  // in the future this is perhaps a descriptive enum
    air = Vector2{125.f, 125.f};
    air_radius = 100.f;
    air_rotation_speed = 0.006f;
    apron_position = Vector2{570.f, 420.f};

    // initialize random stuff once
    std::random_device rd;
    gen = mt19937{rd()};
    flight_num_gen = uniform_int_distribution<int>{flight_number_range.first,
                                                   flight_number_range.second};
    arrival_time_minute_gen = uniform_int_distribution<int>{
        arrival_time_minute_range.first, arrival_time_minute_range.second};
    refuel_time_minute_gen = uniform_int_distribution<int>{
        refuel_time_minute_range.first, refuel_time_minute_range.second};
    apron_id_gen = uniform_int_distribution<int>{
        0,
        apron_count -
            1};  // aprons cannot be added during between init and unload calls
    plane_texture_gen = uniform_int_distribution<int>{0, 11};

    // generate flight schedule
    generateFlightSetA();
    // buttons
    buttons.clear();
    buttons.push_back(Button{.id = 0,
                             .region = Rectangle{600.f, 525.f, 148.f, 36.f},
                             .name = "schedule button",
                             .text = "Schedule",
                             .fontsize = 30,
                             .active = false});
}
void AirControllerScene::update() {
    ++frame_counter;
    updateTime();
    updateFlights();
    processUserInput();
}
void AirControllerScene::draw() {
    drawLighting();
    DrawTexture(background, 0, 0, Fade(WHITE, 0.9f));
    drawGui();
    drawSchedule();
    drawPlanesInAir();
    drawAprons();
    drawTime();
    drawFlightInfo();
    DrawFPS(0, 0);
    if (over) {
        drawScore();
    }
}
void AirControllerScene::unload() {
    UnloadTexture(background);
    for (auto &t : digit_textures) {
        UnloadTexture(t);
    }
    for (auto &[_, plane] : plane_data) {
        UnloadTexture(plane.texture);
    }
    UnloadTexture(odot_texture);
    UnloadTexture(flag_texture);
}
bool AirControllerScene::done() { return done_; }
