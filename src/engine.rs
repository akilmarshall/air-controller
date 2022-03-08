use crate::credit;
use crate::game;
use crate::menu;
use crate::splash;
use crate::tutorial;
use macroquad::prelude::*;

pub fn title() -> String {
    "Air Controller".to_string()
}
pub fn window_width() -> f32 {
    800.
}
pub fn window_height() -> f32 {
    600.
}

#[derive(Copy, Clone, Default)]
pub enum Scene {
    #[default]
    Splash,
    Menu,
    Game,
    Credit,
    Tutorial,
}

#[derive(Default)]
pub struct Engine {
    title: String,
    current: Scene,
    alpha: f32,
    fade_out: bool,
    on_transition: bool,
    from: Scene,
    to: Scene,
    splash: splash::Splash,
}
impl Engine {
    pub fn new() -> Self {
        Default::default()
    }
    pub unsafe fn init(&mut self, s: Scene) {
        self.title = title();
        self.current = Scene::Splash;
        self.alpha = 0.;
        self.fade_out = true;
        self.on_transition = true;
        self.from = Scene::Splash;
        self.to = Scene::Splash;
        self.change(s);
    }
    async unsafe fn init_scene(&mut self) {
        match self.current {
            Scene::Splash => {
                self.splash.init();
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
    async unsafe fn update_scene(&mut self) {
        match self.current {
            Scene::Splash => {
                self.splash.update();
                if self.splash.done() {
                    self.transition(self.splash.next());
                }
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
    async unsafe fn draw_scene(&self) {
        match self.current {
            Scene::Splash => {
                self.splash.draw();
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
    unsafe fn change(&mut self, s: Scene) {
        self.current = s;
        self.init_scene();
    }
    unsafe fn transition(&mut self, s: Scene) {
        self.on_transition = true;
        self.fade_out = false;
        self.from = self.current;
        self.to = s;
        self.alpha = 0.;
    }
    async unsafe fn update_transition(&mut self) {
        if !self.fade_out {
            self.alpha += 0.05;
            if self.alpha > 1.01 {
                self.alpha = 1.;
                self.current = self.to;
                self.fade_out = true;
                self.init_scene().await;
            }
        } else {
            self.alpha -= 0.05;
            if self.alpha < -0.01 {
                self.alpha = 0.;
                self.fade_out = false;
                self.on_transition = false;
            }
        }
    }
    fn draw_transition(&self) {
        draw_rectangle(
            0.,
            0.,
            window_width(),
            window_height(),
            fade(BLACK, self.alpha),
        );
    }
    pub async unsafe fn step(&mut self) {
        if !self.on_transition {
            self.update_scene();
        } else {
            self.update_transition().await;
        }
        self.draw_scene().await;
        if self.on_transition {
            self.draw_transition();
        }
    }
}

// data

static mut CURRENT: Scene = Scene::Splash;
static mut ALPHA: f32 = 0.;
static mut FADE_OUT: bool = true;
static mut ON_TRANSITION: bool = false;
static mut FROM: Scene = Scene::Splash;
static mut TO: Scene = Scene::Splash;

pub fn lerp(a: f32, b: f32, mu: f32) -> f32 {
    a + ((b - a).abs() * mu)
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
