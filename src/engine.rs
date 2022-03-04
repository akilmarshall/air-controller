use crate::credit;
use crate::game;
use crate::menu;
use crate::splash;
use crate::tutorial;
use macroquad::prelude::*;

#[derive(Copy, Clone)]
pub enum Scene {
    Splash,
    Menu,
    Game,
    Credit,
    Tutorial,
}

// data
pub static TITLE: &str = "Air Controller";
pub static WIDTH: f32 = 800.;
pub static HEIGHT: f32 = 600.;

static mut CURRENT: Scene = Scene::Splash;
static mut ALPHA: f32 = 0.;
static mut FADE_OUT: bool = true;
static mut ON_TRANSITION: bool = false;
static mut FROM: Scene = Scene::Splash;
static mut TO: Scene = Scene::Splash;

// api methods
pub async unsafe fn init(s: Scene) -> () {
    ALPHA = 0.;
    FADE_OUT = true;
    ON_TRANSITION = true;
    change(s).await;
}
async unsafe fn init_scene() {
    // function to call scene intialization methods for the CURRENT scene
    match CURRENT {
        Scene::Splash => {
            splash::init();
        }
        Scene::Menu => {
            menu::init().await;
        }
        Scene::Game => {
            game::init().await;
        }
        Scene::Credit => {
            credit::init();
        }
        Scene::Tutorial => {
            tutorial::init();
        }
    }
}
unsafe fn update_scene() {
    match CURRENT {
        Scene::Splash => {
            splash::update();
        }
        Scene::Menu => {
            menu::update();
        }
        Scene::Game => {
            game::update();
        }
        Scene::Credit => {
            credit::update();
        }
        Scene::Tutorial => {
            tutorial::update();
        }
    }
}
async unsafe fn draw_scene() {
    match CURRENT {
        Scene::Splash => {
            splash::draw();
        }
        Scene::Menu => {
            menu::draw().await;
        }
        Scene::Game => {
            game::draw();
        }
        Scene::Credit => {
            credit::draw();
        }
        Scene::Tutorial => {
            tutorial::draw();
        }
    }
}

pub async unsafe fn change(s: Scene) -> () {
    CURRENT = s;
    init_scene().await;
}
pub unsafe fn transition(s: Scene) -> () {
    ON_TRANSITION = true;
    FADE_OUT = false;
    FROM = CURRENT;
    TO = s;
    ALPHA = 0.;
}
pub async unsafe fn update_transition() -> () {
    if !FADE_OUT {
        ALPHA += 0.05;
        if ALPHA > 1.01 {
            ALPHA = 1.;
            CURRENT = TO;
            FADE_OUT = true;
            init_scene().await;
        }
    } else {
        ALPHA -= 0.05;
        if ALPHA < -0.01 {
            ALPHA = 0.;
            FADE_OUT = false;
            ON_TRANSITION = false;
        }
    }
}
pub fn fade(c: Color, a: f32) -> Color {
    // helper functions to construct an arbitrarily transparent color
    Color {
        r: c.r,
        g: c.g,
        b: c.b,
        a,
    }
}
pub unsafe fn draw_transition() -> () {
    draw_rectangle(0., 0., WIDTH, HEIGHT, fade(BLACK, ALPHA));
}
pub async unsafe fn step() -> () {
    if !ON_TRANSITION {
        update_scene();
    } else {
        update_transition().await;
    }
    draw_scene().await;
    if ON_TRANSITION {
        draw_transition();
    }
}
