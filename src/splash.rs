use crate::engine::{window_height, window_width, Scene};
use macroquad::prelude::*;

#[derive(Default)]
pub struct Splash {
    frame_counter: u32,
    t: u32,
    pub next: Scene,
    pub done: bool,
}

impl Splash {
    fn new() -> Self {
        Default::default()
    }
    pub fn init(&mut self) {
        self.frame_counter = 0;
        self.t = 300;
        self.next = Scene::Menu;
        self.done = false;
    }
    pub fn update(&mut self) {
        self.done = self.frame_counter > self.t || is_mouse_button_pressed(MouseButton::Left);
        self.frame_counter += 1;
    }
    fn f(
        &self,
        xlen: f32,
        ylen: f32,
        xspeed: f32,
        yspeed: f32,
        a: f32,
        b: f32,
        xoff: f32,
        yoff: f32,
        color: Color,
    ) {
        let x: f32 = xlen * (((self.frame_counter as f32) * xspeed) + a).cos();
        let y: f32 = ylen * (((self.frame_counter as f32) * yspeed) + b).sin();
        draw_circle(x + xoff, y + yoff, 5.0, color);
        draw_circle(-x + xoff, -y + yoff, 5.0, color);
        draw_line(x + xoff, y + yoff, -x + xoff, -y + yoff, 2.0, color);
    }
    pub fn draw(&self) -> () {
        clear_background(WHITE);
        self.f(
            50.,
            50.,
            0.05,
            0.03,
            0.,
            0.,
            window_width() / 2.,
            200.,
            BLACK,
        );
        self.f(
            50.,
            50.,
            0.01,
            0.03,
            0.5,
            0.5,
            window_width() / 2.,
            200.,
            RED,
        );
        draw_text(
            "Unknown Stud os",
            (window_width() / 2.) - 135.,
            (window_height() / 2.) + 50.,
            40.,
            BLACK,
        );
        draw_text(
            "i",
            (window_width() / 2.) + 75.,
            (window_height() / 2.) + 50.,
            42.,
            RED,
        );
    }
}
