use crate::engine::{window_height, window_width, Scene};
use macroquad::prelude::*;

#[derive(Default)]
pub struct Tutorial {
    frame_counter: u32,
    t: u32,
    pub done: bool,
    pub next: Scene,
}

impl Tutorial {
    pub fn init(&mut self) {
        self.t = 100;
        self.frame_counter = 0;
        self.done = false;
        self.next = Scene::Menu;
    }

    pub fn update(&mut self) {
        self.done = self.frame_counter > self.t && is_mouse_button_pressed(MouseButton::Left);
        self.frame_counter += 1;
    }

    pub fn draw(&self) {
        clear_background(WHITE);
        draw_text("Tutorial", 40.0, 40.0, 40.0, BLACK);

        if self.frame_counter > self.t {
            draw_text(
                "click to continue",
                window_width() / 2.0,
                window_height() / 2.0,
                20.0,
                GRAY,
            )
        }
    }
}
