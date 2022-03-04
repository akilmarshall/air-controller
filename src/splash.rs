use crate::engine::transition;
use crate::engine::{Scene, HEIGHT, WIDTH};
use macroquad::prelude::*;

static mut FRAME_COUNTER: u32 = 0;
static T: u32 = 300;

pub unsafe fn init() -> () {
    FRAME_COUNTER = 0;
}
pub unsafe fn update() -> () {
    if FRAME_COUNTER > T || is_mouse_button_pressed(MouseButton::Left) {
        transition(Scene::Menu);
    }
    FRAME_COUNTER += 1;
}
unsafe fn f(
    xlen: f32,
    ylen: f32,
    xspeed: f32,
    yspeed: f32,
    a: f32,
    b: f32,
    xoff: f32,
    yoff: f32,
    color: Color,
) -> () {
    let x: f32 = xlen * (((FRAME_COUNTER as f32) * xspeed) + a).cos();
    let y: f32 = ylen * (((FRAME_COUNTER as f32) * yspeed) + b).sin();
    draw_circle(x + xoff, y + yoff, 5.0, color);
    draw_circle(-x + xoff, -y + yoff, 5.0, color);
    draw_line(x + xoff, y + yoff, -x + xoff, -y + yoff, 2.0, color);
}
pub unsafe fn draw() -> () {
    clear_background(WHITE);
    f(50., 50., 0.05, 0.03, 0., 0., WIDTH / 2., 200., BLACK);
    f(50., 50., 0.01, 0.03, 0.5, 0.5, WIDTH / 2., 200., RED);
    draw_text(
        "Unknown Stud os",
        (WIDTH / 2.) - 135.,
        (HEIGHT / 2.) + 50.,
        40.,
        BLACK,
    );
    draw_text("i", (WIDTH / 2.) + 75., (HEIGHT / 2.) + 50., 42., RED);
}
