mod credit;
mod db;
mod engine;
mod game;
mod menu;
mod splash;
mod tutorial;
mod util;

use macroquad::prelude::*;

fn conf() -> Conf {
    Conf {
        window_title: engine::title().to_owned(),
        window_width: engine::window_width() as i32,
        window_height: engine::window_height() as i32,
        ..Default::default()
    }
}

#[macroquad::main(conf)]
async fn main() {
    unsafe {
        let mut engine = engine::Engine::new();
        engine.init(engine::Scene::Splash);
        // engine::init(engine::Scene::Splash).await;
        loop {
            engine.step().await;
            // engine::step().await;
            next_frame().await
        }
    }
}
