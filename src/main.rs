mod credit;
mod engine;
mod game;
mod menu;
mod splash;
mod tutorial;
mod util;

use macroquad::prelude::*;

fn conf() -> Conf {
    Conf {
        window_title: engine::TITLE.to_owned(),
        window_width: engine::WIDTH as i32,
        window_height: engine::HEIGHT as i32,
        ..Default::default()
    }
}

#[macroquad::main(conf)]
async fn main() {
    unsafe {
        engine::init(engine::Scene::Splash).await;
        loop {
            engine::step().await;
            next_frame().await
        }
    }
}
