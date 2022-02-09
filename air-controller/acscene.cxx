#include "api.hxx"
#include "control.hxx"
#include "data.hxx"

void api::scene::ACSceneInit() {
    data::ACScene::done = false;
    data::ACScene::frame_counter = 0;
    data::ACScene::MIN = 25;

    data::ACScene::minute = 0;
    data::ACScene::hour = 0;
    data::ACScene::good = 0;
    data::ACScene::ok = 0;
    data::ACScene::bad = 0;
    data::ACScene::background_id =
        0;  // in the future this is perhaps a descriptive enum
    data::ACScene::air = Vector2{125.f, 125.f};
    data::ACScene::air_radius = 100.f;
    data::ACScene::air_rotation_speed = 0.006f;
    data::ACScene::apron_position = Vector2{570.f, 420.f};
    data::ACScene::clock_position =
        Vector2{GetScreenWidth() / 2.f, GetScreenHeight() / 8.f};
    data::ACScene::schedule_area =
        Rectangle{50.f, (GetScreenHeight() / 2.f) - 45, GetScreenWidth() / 2.f,
                  (GetScreenHeight() / 2.f) + 5.f};
    data::ACScene::schedule_fontsize = 20;
    data::ACScene::score_area = Rectangle{
        (3 / 4.f) * GetScreenWidth(), GetScreenHeight() / 8.f, 150.f, 150.f};
    data::ACScene::score_fontsize = 20;
    data::ACScene::highlight = GOLD;
    data::ACScene::apron_count = 3;
    for (int i = 0; i < data::ACScene::apron_count; ++i) {
        data::aprons.push_back(data::Apron{.id = i, .active = false});
    }

    // initialize random stuff once
    data::ACScene::flight_number_range = make_pair(100, 999);
    data::ACScene::arrival_time_minute_range = make_pair(0, 59);
    data::ACScene::refuel_time_minute_range = make_pair(15, 60);
    std::random_device rd;
    data::ACScene::gen = mt19937{rd()};
    data::ACScene::flight_num_gen = uniform_int_distribution<int>{
        data::ACScene::flight_number_range.first,
        data::ACScene::flight_number_range.second};
    data::ACScene::arrival_time_minute_gen = uniform_int_distribution<int>{
        data::ACScene::arrival_time_minute_range.first,
        data::ACScene::arrival_time_minute_range.second};
    data::ACScene::refuel_time_minute_gen = uniform_int_distribution<int>{
        data::ACScene::refuel_time_minute_range.first,
        data::ACScene::refuel_time_minute_range.second};
    data::ACScene::apron_id_gen = uniform_int_distribution<int>{
        0,
        data::ACScene::apron_count -
            1};  // aprons cannot be added during between init and unload calls
    data::ACScene::plane_texture_gen = uniform_int_distribution<int>{0, 11};

    // generate flight schedule
    feeder::generateFlightSetA();
    // buttons
    data::ACScene::buttons.clear();
    data::ACScene::buttons.push_back(
        data::Button{.id = 0,
                     .region = Rectangle{600.f, 525.f, 148.f, 36.f},
                     .name = "schedule button",
                     .text = "Schedule",
                     .fontsize = 30,
                     .active = false,
                     .hovered = false});
}
void api::scene::ACSceneUpdate() {
    ++data::ACScene::frame_counter;
    feeder::updateTime();
    feeder::updateFlights();
    control::state::processUserInput();
}
void api::scene::ACSceneDraw() {
    observer::drawBackground();
    observer::drawButtons();
    observer::drawSelectFlightInfo();
    observer::drawFlights();
    observer::drawAprons();
    observer::drawClock();
    observer::drawScore();
    observer::drawSchedule();
}
void api::scene::ACSceneUnload() {}
bool api::scene::ACSceneDone() {
    static int counter{0};
    if (logic::pure::complete()) {
        ++counter;
    }
    if (counter >= data::ACScene::MIN * 15) {
        return true;
    }
    return false;
}
