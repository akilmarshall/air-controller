use crate::credit;
use crate::db::DB;
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

#[derive(Copy, Clone)]
pub enum Scene {
    Splash,
    Menu,
    Game,
    Credit,
    Tutorial,
}
impl Default for Scene {
    fn default() -> Self {
        Scene::Menu
    }
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
    menu: menu::Menu,
    game: game::Game,
    credit: credit::Credit,
    tutorial: tutorial::Tutorial,
    resources: DB,
}
impl Engine {
    pub fn new() -> Self {
        Default::default()
    }
    pub async fn init(&mut self, s: Scene) {
        self.title = title();
        self.current = Scene::Splash;
        self.alpha = 0.;
        self.fade_out = true;
        self.on_transition = true;
        self.from = Scene::Splash;
        self.to = Scene::Splash;
        self.resources.load_textures().await;
        self.change(s);
    }
    fn init_scene(&mut self) {
        match self.current {
            Scene::Splash => {
                self.splash.init();
            }
            Scene::Menu => {
                if let Some(bg) = self.resources.bg() {
                    self.menu.init(bg);
                }
            }
            Scene::Game => {
                // holy monad batman
                if let Some(bg) = self.resources.bg() {
                    if let Some(flag) = self.resources.flag() {
                        if let Some(odot) = self.resources.odot() {
                            if let Some(planes) = self.resources.planes() {
                                if let Some(digits) = self.resources.digits() {
                                    self.game.init(bg, flag, odot, planes, digits);
                                }
                            }
                        }
                    }
                }
            }
            Scene::Credit => {
                self.credit.init();
            }
            Scene::Tutorial => {
                self.tutorial.init();
            }
        }
    }
    fn update_scene(&mut self) {
        match self.current {
            Scene::Splash => {
                self.splash.update();
                if self.splash.done {
                    self.transition(self.splash.next);
                }
            }
            Scene::Menu => {
                self.menu.update();
                if self.menu.done {
                    self.transition(self.menu.next);
                }
            }
            Scene::Game => {
                self.game.update();
                if self.game.done {
                    self.transition(self.game.next);
                }
            }
            Scene::Credit => {
                self.credit.update();
                if self.credit.done {
                    self.transition(self.credit.next);
                }
            }
            Scene::Tutorial => {
                self.tutorial.update();
                if self.tutorial.done {
                    self.transition(self.tutorial.next);
                }
            }
        }
    }
    fn draw_scene(&self) {
        match self.current {
            Scene::Splash => {
                self.splash.draw();
            }
            Scene::Menu => {
                self.menu.draw();
            }
            Scene::Game => {
                self.game.draw();
            }
            Scene::Credit => {
                self.credit.draw();
            }
            Scene::Tutorial => {
                self.tutorial.draw();
            }
        }
    }
    fn change(&mut self, s: Scene) {
        self.current = s;
        self.init_scene();
    }
    fn transition(&mut self, s: Scene) {
        self.on_transition = true;
        self.fade_out = false;
        self.from = self.current;
        self.to = s;
        self.alpha = 0.;
    }
    fn update_transition(&mut self) {
        if !self.fade_out {
            self.alpha += 0.05;
            if self.alpha > 1.01 {
                self.alpha = 1.;
                self.current = self.to;
                self.fade_out = true;
                self.init_scene();
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
    pub fn step(&mut self) {
        if !self.on_transition {
            self.update_scene();
        } else {
            self.update_transition();
        }
        self.draw_scene();
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
