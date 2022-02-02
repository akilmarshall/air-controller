#include "api.hpp"

// api
void step() { data::scene_manager.Step(); }
// feeders
void api::feeder::generateFlightSetA() {
    auto flight = data::Flight{.id = 1,
                               .arrival_time = 75 * data::ACScene::MIN,
                               .departure_time = 100 * data::ACScene::MIN,
                               .apron_id = 1,
                               .status = data::PRE,
                               .selected = false,
                               .hovered = false};
    data::flights.push_back(flight);
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
void api::feeder::selectFlight(int id) {
    for (auto &flight : data::flights) {
        flight.selected = (flight.id == id);
    }
}
// observers
void api::observer::drawBackground() {
    auto sprite = logic::pure::backgroundSpriteACScene();
    DrawTexture(sprite.texture, 0.f, 0.f, WHITE);
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
        auto flight_sprite = logic::pure::queryFlightSprite(flight_id);
        if (flight_sprite) {
            if (flight) {
                if (flight.value().selected || flight.value().hovered) {
                    auto s = 2;
                    auto dest_scaled = Rectangle{dest};
                    dest_scaled.x -= (s * dest.width) / 2;
                    dest_scaled.y -= (s * dest.height) / 2;
                    dest_scaled.height *= 2;
                    dest_scaled.width *= 2;
                    DrawTexturePro(
                        flight_sprite.value().texture, source, dest_scaled,
                        Vector2{logic::pure::planeSpriteWidth() / 2.f,
                                logic::pure::planeSpriteHeight() / 2.f},
                        0.f, WHITE);
                } else {
                    DrawTexturePro(
                        flight_sprite.value().texture, source, dest,
                        Vector2{logic::pure::planeSpriteWidth() / 2.f,
                                logic::pure::planeSpriteHeight() / 2.f},
                        0.f, WHITE);
                }
            }
        } else {
            // draw default graphics
            DrawRectangleLinesEx(dest, 2.0f, RED);
            DrawText(std::to_string(flight_id).c_str(), dest.x, dest.y, 20,
                     RED);
        }
    }
}
void api::observer::drawAprons() {
    for (auto &[apron_id, pos] : logic::derived::apronPositions()) {
        auto sprite = logic::pure::queryApronSprite(apron_id);
        if (sprite) {
            DrawTexture(sprite.value().texture, pos.x, pos.y, WHITE);
        } else {
            // draw default graphics
            DrawRectangleLinesEx(pos, 2.0f, RED);
            DrawText(std::to_string(apron_id).c_str(), pos.x, pos.y, 20, RED);
        }
    }
    for (auto &[apron_id, pos] : logic::derived::apronFlagPositions()) {
        auto apron = logic::pure::queryApron(apron_id);
        if (apron) {
            auto sprite = logic::pure::queryFlagSprite(apron_id);
            if (sprite) {
                Color c;
                if (apron.value().active) {
                    c = WHITE;
                } else {
                    c = DARKPURPLE;
                }
                DrawTexture(sprite.value().texture, pos.x, pos.y, c);
            } else {
                // draw default graphics
                DrawRectangleLinesEx(pos, 2.f, RED);
                DrawText(apron.active ? "on" : "off", pos.x, pos.y, 20, RED);
            }
        }
    }
}
void api::observer::drawClock() {
    auto s = 2.0f;
    // split hour:min -> ab:cd
    int a = data::ACScene::hour / 10;
    int b = data::ACScene::hour % 10;
    int c = data::ACScene::minute / 10;
    int d = data::ACScene::minute % 10;
    auto source = Rectangle{0.f, 0.f, logic::pure::digitSpriteWidth(),
                            logic::pure::digitSpriteHeight()};
    auto dest_a = Rectangle{
        data::ACScene::clock_position.x - (2 * logic::pure::digitSpriteWidth()),
        data::ACScene::clock_position.y, s * logic::pure::digitSpriteWidth(),
        s * logic::pure::digitSpriteHeight()};
    auto dest_b = Rectangle{
        data::ACScene::clock_position.x - (1 * logic::pure::digitSpriteWidth()),
        data::ACScene::clock_position.y, s * logic::pure::digitSpriteWidth(),
        s * logic::pure::digitSpriteHeight()};
    auto dest_c = Rectangle{
        data::ACScene::clock_position.x + (1 * logic::pure::digitSpriteWidth()),
        data::ACScene::clock_position.y, s * logic::pure::digitSpriteWidth(),
        s * logic::pure::digitSpriteHeight()};
    auto dest_d = Rectangle{
        data::ACScene::clock_position.x + (2 * logic::pure::digitSpriteWidth()),
        data::ACScene::clock_position.y, s * logic::pure::digitSpriteWidth(),
        s * logic::pure::digitSpriteHeight()};
    auto sprite_a = logic::pure::queryDigitSprite(a);
    auto sprite_b = logic::pure::queryDigitSprite(b);
    auto sprite_c = logic::pure::queryDigitSprite(c);
    auto sprite_d = logic::pure::queryDigitSprite(d);
    auto default_draw = [&](Rectangle d) {
        DrawRectangleLines(d, data::ACScene::clock_position.y,
                           logic::pure::digitSpriteWidth(),
                           logic::pure::digitSpriteHeight(), RED);
    };
    if (sprite_a) {
        DrawTexturePro(sprite_a.value().texture, source, dest_a,
                       Vector2{logic::pure::digitSpriteWidth() / 2,
                               logic::pure::digitSpriteHeight() / 2},
                       0.f, WHITE);
    } else {
        // draw default graphics
        default_draw(dest_a);
        DrawText(std::to_string(a).c_str(), dest_a.x,
                 data::ACScene::clock_position.y, 20, RED);
    }
    if (sprite_b) {
        DrawTexturePro(sprite_b.value().texture, source, dest_b,
                       Vector2{logic::pure::digitSpriteWidth() / 2,
                               logic::pure::digitSpriteHeight() / 2},
                       0.f, WHITE);
    } else {
        // draw default graphics
        default_draw(dest_b);
        DrawText(std::to_string(b).c_str(), dest_b.x,
                 data::ACScene::clock_position.y, 20, RED);
    }
    if (sprite_c) {
        DrawTexturePro(sprite_c.value().texture, source, dest_c,
                       Vector2{logic::pure::digitSpriteWidth() / 2,
                               logic::pure::digitSpriteHeight() / 2},
                       0.f, WHITE);
    } else {
        // draw default graphics
        default_draw(dest_c);
        DrawText(std::to_string(c).c_str(), dest_c.x,
                 data::ACScene::clock_position.y, 20, RED);
    }
    if (sprite_d) {
        DrawTexturePro(sprite_d.value().texture, source, dest_d,
                       Vector2{logic::pure::digitSpriteWidth() / 2,
                               logic::pure::digitSpriteHeight() / 2},
                       0.f, WHITE);
    } else {
        // draw default graphics
        default_draw(dest_d);
        DrawText(std::to_string(d).c_str(), dest_d.x,
                 data::ACScene::clock_position.y, 20, RED);
    }
    DrawTexturePro(data::odot_sprite.texture, source,
                   Rectangle{data::ACScene::clock_position.x,
                             data::ACScene::clock_position.y,
                             logic::pure::digitSpriteWidth(),
                             logic::pure::digitSpriteHeight()},
                   Vector2{logic::pure::digitSpriteWidth() / 2,
                           logic::pure::digitSpriteHeight() / 2},
                   0.f, WHITE);
    DrawTexturePro(data::odot_sprite.texture, source,
                   Rectangle{data::ACScene::clock_position.x,
                             data::ACScene::clock_position.y - 12,
                             logic::pure::digitSpriteWidth(),
                             logic::pure::digitSpriteHeight()},
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
        auto col1 = data::ACScene::schedule_area.x + 10;
        auto col2 = data::ACScene::schedule_area.x + 140;
        auto col3 = data::ACScene::schedule_area.x + 260;
        auto col4 = data::ACScene::schedule_area.x + 400;
        auto y = data::ACScene::schedule_area.height;
        DrawRectangleRounded(data::ACScene::schedule_area, 0.1, 100,
                             Fade(DARKGRAY, 0.8f));
        DrawText("Flight #", col1, y, 20, GOLD);
        DrawText("Arrival", col2, y, 20, GOLD);
        DrawText("Departure", col3, y, 20, GOLD);
        DrawRectangle(col1, y + 25, data::ACScene::schedule_area.width - 20, 4,
                      GOLD);
        for (int i = 0; auto &flight : data::ACScene::flight_schedule) {
            switch (flight.status) {
                case data::AIR || data::APRON: {
                    DrawText(std::to_string(flight.id).c_str(), col1,
                             (y - 30) + i * 25,
                             data::ACScene::schedule_fontsize, GOLD);
                    // decompose arrival time into ab:cd
                    auto t = logic::pure::minuteToDigits(flight.arrival_time /
                                                         data::ACScene::MIN);
                    auto &[a, b, c, d] = t;
                    DrawText(TextFormat("%i%i:%i%i", a, b, c, d), col2,
                             (y - 30) + i * 25, 20, GOLD);
                    t = logic::pure::minuteToDigits(flight.departure_time /
                                                    data::ACScene::MIN);
                    auto &[h, k, v, w] = t;
                    DrawText(TextFormat("%i%i:%i%i", h, k, v, w), col3,
                             (y - 30) + i * 25, 20, GOLD);
                    if (flight.status == data::APRON) {
                        auto apron = logic::pure::queryApronFlightId(flight.id);
                        Color c;
                        if (apron) {
                            if (flight.id == apron.value().flight_id) {
                                c = GREEN;
                            } else {
                                c = RED;
                            }
                        }
                        DrawText(
                            TextFormat(
                                "G%i",
                                logic::pure::queryApronFlightId(flight.id) + 1),
                            col4, (y - 35) + i * 25, 20, c);
                    } else if (flight.status == data::AIR) {
                        auto sprite = logic::pure::queryFlightSprite(flight.id);
                        if (sprite) {
                            /* Draw */
                            DrawTexture(sprite.value().texture, col4,
                                        (y - 35) + i * 25, WHITE);
                        } else {
                            // draw default graphics
                            DrawRectangleLines(col4, (y - 35) + i * 25,
                                               logic::pure::planeSpriteWidth(),
                                               logic::pure::planeSpriteHeight(),
                                               RED);
                        }
                    }
                    auto hover_area = Rectangle{
                        data::ACScene::schedule_area.x, (y - 32) + i * 25,
                        data::ACScene::schedule_area.width, 25};
                    if (logic::pure::isHover(hover_area)) {
                        DrawRectangleLinesEx(hover_area, 2.f, GOLD);
                    }
                } break;
                case data::PRE: {
                } break;
                case data::DONE: {
                } break;
            }
            ++i;
            if (i > 12) break;
        }
    }
}

