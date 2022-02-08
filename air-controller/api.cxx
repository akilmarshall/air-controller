#include "api.hxx"

void api::scene::init(data::scene::Scene next) {
    data::scene::alpha = 0.f;
    data::scene::transition_fade_out = true;
    data::scene::current = next;
    switch (data::scene::current) {
        case data::scene::RAYANIM: {
        } break;
        case data::scene::SPLASH: {
        } break;
        case data::scene::AIRCONTROLLER: {
            api::scene::ACSceneInit();
        } break;
        case data::scene::CREDIT: {
        } break;
    }
}
void api::scene::transition(data::scene::Scene scene) {
    data::scene::on_transition = true;
    data::scene::transition_fade_out = false;
    data::scene::from_scene = data::scene::current;
    data::scene::to_scene = scene;
    data::scene::alpha = 0.f;
}
void api::scene::updateTransition() {
    if (data::scene::transition_fade_out) {
        data::scene::alpha += 0.05;
        if (data::scene::alpha > 1.01f) {
            data::scene::alpha = 1.0f;
            // unload used to be called here
            // load next scene
            switch (data::scene::to_scene) {
                case data::scene::RAYANIM: {
                } break;
                case data::scene::SPLASH: {
                } break;
                case data::scene::AIRCONTROLLER: {
                    api::scene::ACSceneInit();
                } break;
                case data::scene::CREDIT: {
                } break;
            }
            data::scene::current = data::scene::to_scene;
            data::scene::transition_fade_out = true;
        }
    } else {
        data::scene::alpha -= 0.02;
        if (data::scene::alpha < -0.01f) {
            data::scene::alpha = 0.f;
            data::scene::transition_fade_out = false;
            data::scene::on_transition = false;
        }
    }
}
void api::scene::drawTransition() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                  Fade(BLACK, data::scene::alpha));
}

void api::scene::step() {
    if (!data::scene::on_transition) {
        switch (data::scene::current) {
            case data::scene::RAYANIM: {
            } break;
            case data::scene::SPLASH: {
            } break;
            case data::scene::AIRCONTROLLER: {
                api::scene::ACSceneUpdate();
                if (api::scene::ACSceneDone()) {
                    api::scene::transition(data::scene::AIRCONTROLLER);
                }
            } break;
            case data::scene::CREDIT: {
            } break;
        }
    } else {
        api::scene::updateTransition();
    }
    BeginDrawing();
    switch (data::scene::current) {
        case data::scene::RAYANIM: {
        } break;
        case data::scene::SPLASH: {
        } break;
        case data::scene::AIRCONTROLLER: {
            api::scene::ACSceneDraw();
        } break;
        case data::scene::CREDIT: {
        } break;
    }
    if (data::scene::on_transition) {
        api::scene::drawTransition();
    }
    EndDrawing();
}
// feeders
void api::feeder::generateFlightSetA() {
    data::flights.push_back(
        data::Flight{.id = 1,
                     .arrival_time = 75 * data::ACScene::MIN,
                     .departure_time = 175 * data::ACScene::MIN,
                     .apron_id = 1,
                     .status = data::PRE,
                     .selected = false,
                     .hovered = false});
    data::flights.push_back(
        data::Flight{.id = 2,
                     .arrival_time = 105 * data::ACScene::MIN,
                     .departure_time = 205 * data::ACScene::MIN,
                     .apron_id = 2,
                     .status = data::PRE,
                     .selected = false,
                     .hovered = false});
    data::flights.push_back(
        data::Flight{.id = 3,
                     .arrival_time = 130 * data::ACScene::MIN,
                     .departure_time = 230 * data::ACScene::MIN,
                     .apron_id = 3,
                     .status = data::PRE,
                     .selected = false,
                     .hovered = false});
}
void api::feeder::updateTime() {
    auto x = (data::ACScene::frame_counter + 1) % data::ACScene::MIN;
    if (x == 1) {
        data::ACScene::minute += 1;
        if (data::ACScene::minute == 60) {
            data::ACScene::minute = 0;
            data::ACScene::hour += 1;
            if (data::ACScene::hour > 23) {
                data::ACScene::hour = 0;
            }
        }
    }
}
void api::feeder::updateFlights() {
    for (auto &flight : data::flights) {
        switch (flight.status) {
            case data::PRE: {
                if (logic::constraint::showFlight(flight)) {
                    flight.status = data::AIR;
                }
            } break;
            case data::AIR: {
            } break;
            case data::APRON: {
            } break;
            case data::DONE: {
            } break;
        }
    }
}
void api::feeder::setFlightSelect(int id, bool state) {
    // set the selected status for the flight with id
    // iterate each flight and set its select status
    for (auto &flight : data::flights) {
        if (flight.id == id) {
            flight.selected = state;
        }
    }
}
void api::feeder::setFlightSelect(int id) {
    // set the selected status for the flight with id
    // iterate each flight and set its select status
    for (auto &flight : data::flights) {
        if (flight.id == id) {
            flight.selected = !flight.selected;
        }
    }
}
void api::feeder::setFlightHover(int id, bool state) {
    // set the hovered status for the flight with id
    // iterate each flight and set its hovered status
    for (auto &flight : data::flights) {
        /* flight.hovered = (flight.id == id); */
        if (flight.id == id) {
            flight.hovered = state;
            return;
        }
    }
}
void api::feeder::selectApron(int id) {
    // set the selected status for the apron with id
    // iterate the aprons and modify the state of only 1 or fewer aprons
    for (auto &apron : data::aprons) {
        apron.selected = (apron.id == id);
    }
}
// observers
void api::observer::drawBackground() {
    DrawTexture(logic::pure::backgroundTextureACScene(), 0, 0,
                Fade(WHITE, 1.f));
}
void api::observer::drawButtons() {
    for (auto &button : data::ACScene::buttons) {
        Color c;
        if (button.active) {
            c = LIGHTGRAY;
        } else if (button.hovered) {
            c = GRAY;
        } else {
            c = DARKGRAY;
        }
        DrawRectangleRounded(button.region, 0.4, 100,
                             c);  // primary button region
        // button shadow
        auto region = Rectangle{button.region};
        region.x -= 2;
        region.y -= 2;
        region.width += 4;
        region.height += 4;
        DrawRectangleRounded(region, 0.4, 100, Fade(DARKGRAY, 0.6f));

        // TODO compute offsets to center buttons
        auto dx = 0;
        auto dy = 0;
        DrawText(button.text.c_str(), button.region.x + dx,
                 button.region.y + dy, button.fontsize, GOLD);
    }
}
void api::observer::drawFlights() {
    for (auto &[flight_id, pos] : logic::derived::arielPositions()) {
        auto source =
            Rectangle{0.f, 0.f, (float)logic::pure::planeSpriteWidth(),
                      (float)logic::pure::planeSpriteHeight()};
        auto dest =
            Rectangle{pos.x, pos.y, (float)logic::pure::planeSpriteWidth(),
                      (float)logic::pure::planeSpriteHeight()};
        auto flight = logic::pure::queryFlight(flight_id);
        auto flight_texture = logic::pure::queryFlightTexture(flight_id);
        if (flight_texture) {
            if (flight) {
                if (flight.value().selected || flight.value().hovered) {
                    auto s = 2;
                    auto dest_scaled = Rectangle{dest};
                    dest_scaled.x -= dest.width * s / 2;
                    dest_scaled.y -= dest.height * s / 2;
                    dest_scaled.height *= s;
                    dest_scaled.width *= s;
                    DrawTexturePro(
                        flight_texture.value(), source, dest_scaled, Vector2{},
                        /* Vector2{logic::pure::planeSpriteWidth() / 2.f, */
                        /*         logic::pure::planeSpriteHeight() / 2.f}, */
                        0.f, flight->selected ? GOLD : WHITE);
                } else {
                    DrawTexturePro(
                        flight_texture.value(), source, dest,
                        Vector2{logic::pure::planeSpriteWidth() / 2.f,
                                logic::pure::planeSpriteHeight() / 2.f},
                        0.f, WHITE);
                }
            } else {
                // draw default graphics
                DrawRectangleLinesEx(dest, 2.0f, ORANGE);
                DrawText(std::to_string(flight_id).c_str(), dest.x, dest.y, 20,
                         RED);
            }
        } else {
            // draw default graphics
            DrawRectangleLinesEx(dest, 2.0f, RED);
            DrawText(std::to_string(flight_id).c_str(), dest.x, dest.y, 20,
                     RED);
        }
    }
}
void api::observer::drawSelectFlightInfo() {
    auto fontsize = 20;
    auto row1 = data::ACScene::air.y - (2 * 25);
    auto row2 = data::ACScene::air.y - (1 * 25);
    auto row3 = data::ACScene::air.y + (0 * 25);
    auto row4 = data::ACScene::air.y + (1 * 25);
    auto col = data::ACScene::air.x + 10;
    auto flight_ = "Flight#";
    auto arrive = "Arrive";
    auto depart = "Depart";
    auto gate = "Gate";
    std::array<int, 4> widths = {
        MeasureText(flight_, fontsize), MeasureText(arrive, fontsize),
        MeasureText(depart, fontsize), MeasureText(gate, fontsize)};
    auto max_width = max(widths[1], max(widths[2], widths[3]));
    auto shadow =
        Rectangle{col - max_width - 10, row1 - 10, max_width * 2.1f, 110.f};

    for (auto &flight : data::flights) {
        if (flight.selected) {
            DrawRectangleRounded(shadow, 0.4f, 100, Fade(DARKGRAY, 0.8f));
            DrawText(flight_, col - widths[0], row1, fontsize, GOLD);
            DrawText(arrive, col - widths[1], row2, fontsize, GOLD);
            DrawText(depart, col - widths[2], row3, fontsize, GOLD);
            DrawText(gate, col - widths[3], row4, fontsize, GOLD);
            auto x = logic::pure::minuteToDigits(flight.arrival_time /
                                                 data::ACScene::MIN);
            auto [a, b, c, d] = x;
            auto y = logic::pure::minuteToDigits(flight.departure_time /
                                                 data::ACScene::MIN);
            auto [l, m, n, o] = y;
            DrawText(TextFormat("%i", flight.id), col + 10, row1, fontsize,
                     GOLD);
            DrawText(TextFormat("%i%i:%i%i", a, b, c, d), col + 10, row2,
                     fontsize, GOLD);
            DrawText(TextFormat("%i%i:%i%i", l, m, n, o), col + 10, row3,
                     fontsize, GOLD);
            DrawText(TextFormat("%i", flight.apron_id), col + 10, row4,
                     fontsize, GOLD);
        }
    }
}
void api::observer::drawAprons() {
    for (auto &[apron_id, pos] : logic::derived::apronPositions()) {
        auto texture = logic::pure::queryApronTexture(apron_id);
        if (texture) {
            DrawTexture(texture.value(), pos.x, pos.y, WHITE);
        } else {
            // draw default graphics
            DrawRectangleLinesEx(pos, 2.0f, RED);
            DrawText(std::to_string(apron_id + 1).c_str(), pos.x, pos.y, 20,
                     RED);
        }
    }
    for (auto &[apron_id, pos] : logic::derived::apronFlagPositions()) {
        auto apron = logic::pure::queryApron(apron_id);
        if (apron) {
            auto texture = logic::pure::queryFlagTexture(apron_id);
            if (texture) {
                Color c;
                if (apron.value().active) {
                    c = WHITE;
                } else {
                    c = DARKPURPLE;
                }
                DrawTexture(texture.value(), pos.x, pos.y, c);
            } else {
                // draw default graphics
                DrawRectangleLinesEx(pos, 2.f, RED);
                DrawText(apron.value().active ? "on" : "off", pos.x, pos.y, 20,
                         RED);
            }
        }
    }
}
// drawTime
void api::observer::drawClock() {
    auto s = 2.0f;
    // split hour:min -> ab:cd
    int a = data::ACScene::hour / 10;
    int b = data::ACScene::hour % 10;
    int c = data::ACScene::minute / 10;
    int d = data::ACScene::minute % 10;
    auto source = Rectangle{0.f, 0.f, logic::pure::digitSpriteWidth(),
                            logic::pure::digitSpriteHeight()};
    auto dest_a = Rectangle{data::ACScene::clock_position.x -
                                (2 * logic::pure::digitSpriteWidth()) - 8,
                            data::ACScene::clock_position.y,
                            s * logic::pure::digitSpriteWidth(),
                            s * logic::pure::digitSpriteHeight()};
    auto dest_b = Rectangle{data::ACScene::clock_position.x -
                                (1 * logic::pure::digitSpriteWidth()) - 2,
                            data::ACScene::clock_position.y,
                            s * logic::pure::digitSpriteWidth(),
                            s * logic::pure::digitSpriteHeight()};
    auto dest_c = Rectangle{data::ACScene::clock_position.x +
                                (1 * logic::pure::digitSpriteWidth()) + 2,
                            data::ACScene::clock_position.y,
                            s * logic::pure::digitSpriteWidth(),
                            s * logic::pure::digitSpriteHeight()};
    auto dest_d = Rectangle{data::ACScene::clock_position.x +
                                (2 * logic::pure::digitSpriteWidth()) + 8,
                            data::ACScene::clock_position.y,
                            s * logic::pure::digitSpriteWidth(),
                            s * logic::pure::digitSpriteHeight()};
    auto texture_a = logic::pure::queryDigitTexture(a);
    auto texture_b = logic::pure::queryDigitTexture(b);
    auto texture_c = logic::pure::queryDigitTexture(c);
    auto texture_d = logic::pure::queryDigitTexture(d);
    auto default_draw = [&](Rectangle d) { DrawRectangleLinesEx(d, 2.f, RED); };
    if (texture_a) {
        DrawTexturePro(texture_a.value(), source, dest_a,
                       Vector2{logic::pure::digitSpriteWidth() / 2,
                               logic::pure::digitSpriteHeight() / 2},
                       0.f, WHITE);
    } else {
        // draw default graphics
        default_draw(dest_a);
        DrawText(std::to_string(a).c_str(), dest_a.x,
                 data::ACScene::clock_position.y, 20, RED);
    }
    if (texture_b) {
        DrawTexturePro(texture_b.value(), source, dest_b,
                       Vector2{logic::pure::digitSpriteWidth() / 2,
                               logic::pure::digitSpriteHeight() / 2},
                       0.f, WHITE);
    } else {
        // draw default graphics
        default_draw(dest_b);
        DrawText(std::to_string(b).c_str(), dest_b.x,
                 data::ACScene::clock_position.y, 20, RED);
    }
    if (texture_c) {
        DrawTexturePro(texture_c.value(), source, dest_c,
                       Vector2{logic::pure::digitSpriteWidth() / 2,
                               logic::pure::digitSpriteHeight() / 2},
                       0.f, WHITE);
    } else {
        // draw default graphics
        default_draw(dest_c);
        DrawText(std::to_string(c).c_str(), dest_c.x,
                 data::ACScene::clock_position.y, 20, RED);
    }
    if (texture_d) {
        DrawTexturePro(texture_d.value(), source, dest_d,
                       Vector2{logic::pure::digitSpriteWidth() / 2,
                               logic::pure::digitSpriteHeight() / 2},
                       0.f, WHITE);
    } else {
        // draw default graphics
        default_draw(dest_d);
        DrawText(std::to_string(d).c_str(), dest_d.x,
                 data::ACScene::clock_position.y, 20, RED);
    }
    DrawTexturePro(data::textures["odot"], source,
                   Rectangle{data::ACScene::clock_position.x +
                                 (logic::pure::digitSpriteWidth() / 2),
                             data::ACScene::clock_position.y - 6,
                             logic::pure::digitSpriteWidth() * 2,
                             logic::pure::digitSpriteHeight() * 2},
                   Vector2{logic::pure::digitSpriteWidth(),
                           logic::pure::digitSpriteHeight()},
                   0.f, WHITE);
    DrawTexturePro(data::textures["odot"], source,
                   Rectangle{data::ACScene::clock_position.x,
                             data::ACScene::clock_position.y + 2,
                             logic::pure::digitSpriteWidth() * 2,
                             logic::pure::digitSpriteHeight() * 2},
                   Vector2{logic::pure::digitSpriteWidth() / 2,
                           logic::pure::digitSpriteHeight() / 2},
                   0.f, WHITE);
}
void api::observer::drawScore() {
    auto delta_y = data::ACScene::score_area.height / 3.f;
    DrawRectanglePro(data::ACScene::score_area, Vector2{}, 0.f,
                     Fade(DARKGRAY, 0.8f));
    DrawText(TextFormat("Good: %i", data::ACScene::good),
             data::ACScene::score_area.x,
             data::ACScene::score_area.y + (0 * delta_y),
             data::ACScene::score_fontsize, GOLD);
    DrawText(TextFormat("OK: %i", data::ACScene::ok),
             data::ACScene::score_area.x,
             data::ACScene::score_area.y + (1 * delta_y),
             data::ACScene::score_fontsize, GOLD);
    DrawText(TextFormat("Bad: %i", data::ACScene::bad),
             data::ACScene::score_area.x,
             data::ACScene::score_area.y + (2 * delta_y),
             data::ACScene::score_fontsize, GOLD);
}
void api::observer::drawSchedule() {
    if (logic::pure::scheduleActive()) {
        auto x = data::ACScene::schedule_area.x;
        auto y = data::ACScene::schedule_area.y + 4;
        auto col1 = x + 10;
        auto col2 = x + 110;
        auto col3 = x + 200;
        auto col4 = x + 330;
        DrawRectangleRounded(data::ACScene::schedule_area, 0.1, 100,
                             Fade(DARKGRAY, 0.8f));
        DrawText("Flight #", col1, y, 20, GOLD);
        DrawText("Arrival", col2, y, 20, GOLD);
        DrawText("Departure", col3, y, 20, GOLD);
        DrawText("Gate", col4, y, 20, GOLD);
        DrawRectangle(col1, y + 21, data::ACScene::schedule_area.width - 20, 4,
                      GOLD);

        for (int i = 0; auto &flight : data::flights) {
            auto yi = (y + 30) + i * 25;
            switch (flight.status) {
                case data::PRE:
                case data::AIR:
                case data::APRON: {
                    // (col1) Flight #
                    // draw plane sprite
                    auto texture = logic::pure::queryFlightTexture(flight.id);
                    if (texture) {
                        // Draw
                        DrawTexture(
                            texture.value(), col1, yi - 8,
                            Fade(WHITE,
                                 flight.status == data::PRE ? 0.4f : 1.f));
                    } else {
                        // draw default graphics
                        DrawRectangleLines(
                            col1 + 30, yi - 8, logic::pure::planeSpriteWidth(),
                            logic::pure::planeSpriteHeight(), RED);
                    }
                    // draw flight number
                    DrawText(std::to_string(flight.id).c_str(), col1 + 40, yi,
                             data::ACScene::schedule_fontsize, GOLD);
                    // (col2) Arrival
                    // decompose arrival time into ab:cd
                    auto t = logic::pure::minuteToDigits(flight.arrival_time /
                                                         data::ACScene::MIN);
                    auto &[a, b, c, d] = t;
                    DrawText(TextFormat("%i%i:%i%i", a, b, c, d), col2, yi, 20,
                             GOLD);
                    // (col3) Departure
                    t = logic::pure::minuteToDigits(flight.departure_time /
                                                    data::ACScene::MIN);
                    auto &[h, k, v, w] = t;
                    DrawText(TextFormat("%i%i:%i%i", h, k, v, w), col3, yi, 20,
                             GOLD);
                    // (col4) Gate
                    if (flight.status == data::PRE ||
                        flight.status == data::AIR) {
                        DrawText(TextFormat("%i", flight.apron_id + 1), col4,
                                 yi, 20, GOLD);
                    } else if (flight.status == data::APRON) {
                        // TODO
                        // check if flight schedule apron is the apron it was
                        // parked at inform the player what the game thinks
                        // about their decision :)
                    }
                    auto hover_area =
                        Rectangle{data::ACScene::schedule_area.x, yi - 4,
                                  data::ACScene::schedule_area.width, 25};
                    if (logic::pure::isHover(hover_area)) {
                        DrawRectangleLinesEx(hover_area, 2.f, GOLD);
                    }
                } break;
                case data::DONE: {
                } break;
            }

            ++i;
            if (i > 12) break;
        }
    }
}

