use crate::engine::{transition, Scene, HEIGHT, WIDTH};
use macroquad::prelude::*;

static T: u32 = 300;
static mut FRAME_COUNTER: u32 = 0;

pub unsafe fn init() {
    FRAME_COUNTER = 0;
}
pub unsafe fn update() {
    if FRAME_COUNTER > T && is_mouse_button_pressed(MouseButton::Left) {
        transition(Scene::Menu);
    }
    FRAME_COUNTER += 1;
}

pub unsafe fn draw() {
    clear_background(WHITE);
    draw_text("Tutorial", 40., 40., 40., BLACK);

    if FRAME_COUNTER > T {
        draw_text(
            "left click to continue",
            WIDTH / 2.,
            HEIGHT / 2.,
            20.0,
            GRAY,
        )
    }
}
