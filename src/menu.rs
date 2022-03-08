use crate::engine::Scene;
use macroquad::prelude::*;
use macroquad::ui::*;

static TITLE: &str = "Air Controller";
static TITLE_FONT_SIZE: f32 = 40.;

#[derive(Default)]
pub struct Menu {
    title: String,
    title_font_size: f32,
    next: Scene,
}

impl Menu {
    fn new() -> Self {
        Default::default()
    }
    pub fn init() {}
    pub fn update(&mut self) {
        if root_ui().button(Vec2::new(x(), y() + 40.), "Play") {
            self.next = Scene::Game;
        }
        if root_ui().button(Vec2::new(x(), y() + 80.), "Tutorial") {
            self.next = Scene::Tutorial;
        }
        if root_ui().button(Vec2::new(x(), y() + 120.), "Credit") {
            self.next = Scene::Credit;
        }
    }
    pub fn draw() {
        clear_background(WHITE);
        // how the f do i get the textures from here?
        draw_text(TITLE, x(), y(), TITLE_FONT_SIZE, GOLD);
    }
}

pub static mut BG: Option<Texture2D> = None;

fn x() -> f32 {
    50.
}
fn y() -> f32 {
    (HEIGHT / 2.) - 95.
}

pub async unsafe fn draw() {
    clear_background(WHITE);
    match BG {
        Some(t) => {
            draw_texture(t, 0., 0., WHITE);
        }
        None => {}
    }
    // clear_background(WHITE);
    draw_text(TITLE, x(), y(), TITLE_FONT_SIZE, GOLD);
}
