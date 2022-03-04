use crate::engine::{transition, Scene, HEIGHT};
use macroquad::prelude::*;
use macroquad::ui::*;

static TITLE: &str = "Air Controller";
static TITLE_FONT_SIZE: f32 = 40.;
pub static mut BG: Option<Texture2D> = None;

fn x() -> f32 {
    50.
}
fn y() -> f32 {
    (HEIGHT / 2.) - 95.
}

pub async unsafe fn init() {
    BG = Some(load_texture("resource/ito.png").await.unwrap());
    /*
    match BG {
        Some(_) => {}
        None => {}
    }
    */
}
pub unsafe fn update() {
    if root_ui().button(Vec2::new(x(), y() + 40.), "Play") {
        transition(Scene::Game);
    }
    if root_ui().button(Vec2::new(x(), y() + 80.), "Tutorial") {
        transition(Scene::Tutorial);
    }
    if root_ui().button(Vec2::new(x(), y() + 120.), "Credit") {
        transition(Scene::Credit);
    }
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
