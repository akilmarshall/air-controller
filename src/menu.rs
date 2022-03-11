use crate::engine::{window_height, Scene};
use macroquad::prelude::*;
use macroquad::ui::*;
use std::rc::Rc;

static TITLE: &str = "Air Controller";
static TITLE_FONT_SIZE: f32 = 40.;

#[derive(Default)]
pub struct Menu {
    title: String,
    title_font_size: f32,
    pub next: Scene,
    pub done: bool,
    bg: Option<Rc<Texture2D>>,
}

impl Menu {
    async fn new() -> Self {
        Default::default()
    }
    pub fn init(&mut self, bg: Rc<Texture2D>) {
        self.bg = Some(bg);
        self.next = Scene::Menu;
        self.done = false;
    }
    pub fn update(&mut self) {
        if root_ui().button(Vec2::new(x(), y() + 40.), "Play") {
            self.next = Scene::Game;
            self.done = true;
        }
        if root_ui().button(Vec2::new(x(), y() + 80.), "Tutorial") {
            self.next = Scene::Tutorial;
            self.done = true;
        }
        if root_ui().button(Vec2::new(x(), y() + 120.), "Credit") {
            self.next = Scene::Credit;
            self.done = true;
        }
    }
    pub fn draw(&self) {
        clear_background(WHITE);
        if let Some(texture) = &self.bg {
            draw_texture(**texture, 0., 0., WHITE)
        }
        draw_text(TITLE, x(), y(), TITLE_FONT_SIZE, GOLD);
    }
}

fn x() -> f32 {
    50.
}

fn y() -> f32 {
    (window_height() / 2.) - 95.
}
