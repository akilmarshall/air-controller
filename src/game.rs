use crate::engine::{fade, lerp, transition, Scene, HEIGHT, WIDTH};
use crate::menu::BG;
use crate::util::collision;
use macroquad::math::clamp;
use macroquad::prelude::*;
use macroquad::texture::DrawTextureParams;
use macroquad::ui::*;
use std::f32::consts::PI;

static mut FRAME_COUNTER: u32 = 0;
static mut PLANE_TEXTURES: Option<Vec<Texture2D>> = None;
static mut DIGIT_TEXTURES: Option<Vec<Texture2D>> = None;
static mut ODOT_TEXTURE: Option<Texture2D> = None;
static mut FLAG_TEXTURE: Option<Texture2D> = None;

static MIN: u32 = 25;
static mut SCHEDULE_ACTIVE: bool = false;
static mut FLIGHTS: Option<Vec<Flight>> = None;
static mut GOOD: i32 = 0;
static mut OK: i32 = 0;
static mut BAD: i32 = 0;
static CENTER: (f32, f32) = (125., 125.);
static RADIUS: f32 = 100.;
static SPEED: f32 = 0.006;

struct Data {
    frame_counter: u32,
    min: u32,
    schedule_active: bool,
    flights: Vec<Flight>,
    good: u32,
    ok: u32,
    bad: u32,
    center: (f32, f32),
    radius: f32,
    speed: f32,
}

#[derive(PartialEq, Eq)]
enum FlightStatus {
    Pre,
    Air,
    Apron(u32),
    Done,
}

struct Flight {
    id: u32,
    arrival: u32,         // scheduled time to arrive in frames
    departure: u32,       // scheduled time to depart in frames
    apron: u32,           // scheduled apron to depart from
    status: FlightStatus, // status of the flight
    sprite: u32,          // flight texture id
}

// describe all types of parametric, paths a plane can take
enum PathType {
    Holding(u32, u32), //  (i, n), Circular holding path, ith plane of n
}
impl PathType {
    unsafe fn pos(&self) -> (f32, f32) {
        // implements a parameteric position equation for each PathType
        match &self {
            PathType::Holding(i, n) => {
                // circular holding path
                let theta: f32 = (SPEED * FRAME_COUNTER as f32)
                    + (i.to_owned() as f32 * 2. * PI / n.to_owned() as f32);
                (
                    CENTER.0 + (RADIUS * theta.cos()),
                    CENTER.1 + (RADIUS * theta.sin()),
                )
            }
        }
    }
}
struct FlightPath {
    default: PathType,
    next: PathType,
    start: u32,
    end: u32,
}
impl FlightPath {
    unsafe fn mu(&self) -> f32 {
        // mu the interpolation parameter
        // 0 <= mu <= 1
        let duration = self.end - self.start;
        let t = self.end - FRAME_COUNTER;
        clamp(t as f32 / duration as f32, 0., 1.)
    }
    unsafe fn pos(&self) -> (f32, f32) {
        // compute the position by linealy interpolating the default and next positions
        let d = self.default.pos();
        let n = self.next.pos();
        (lerp(d.0, n.0, self.mu()), lerp(d.1, n.1, self.mu()))
    }
}

pub async unsafe fn init() {
    /*
     */
    FRAME_COUNTER = 0;
    SCHEDULE_ACTIVE = false;
    FLIGHTS = Some(test_flight_set());
    GOOD = 0;
    OK = 0;
    BAD = 0;
    match PLANE_TEXTURES {
        Some(_) => {}
        None => {
            let mut planes = Vec::new();
            for i in 0..11 {
                planes.push(
                    load_texture(format!("resource/ship_{}.png", i).as_str())
                        .await
                        .unwrap(),
                );
            }
            PLANE_TEXTURES = Some(planes);
        }
    }
    match DIGIT_TEXTURES {
        Some(_) => {}
        None => {
            let mut digits = Vec::new();
            for i in 0..10 {
                digits.push(
                    load_texture(format!("resource/digit_{}.png", i).as_str())
                        .await
                        .unwrap(),
                );
            }
            DIGIT_TEXTURES = Some(digits);
        }
    }
    match ODOT_TEXTURE {
        Some(_) => {}
        None => {
            ODOT_TEXTURE = Some(load_texture("resource/odot.png").await.unwrap());
        }
    }
    match FLAG_TEXTURE {
        Some(_) => {}
        None => {
            FLAG_TEXTURE = Some(load_texture("resource/flag.png").await.unwrap());
        }
    }
}
pub unsafe fn update() {
    // schedule button
    if root_ui().button(Vec2::new(600., 450.), "Flight Schedule") {
        SCHEDULE_ACTIVE = !SCHEDULE_ACTIVE;
    }
    if is_mouse_button_pressed(MouseButton::Right) {
        transition(Scene::Menu);
    }
    FRAME_COUNTER += 1;
}
pub unsafe fn draw() {
    match BG {
        Some(t) => {
            draw_texture(t, 0., 0., WHITE);
        }
        None => {
            clear_background(WHITE);
        }
    }
    draw_schedule();
    draw_clock();
    draw_score();
    draw_flights();
}

unsafe fn draw_schedule() {
    let x = 50.;
    let y = HEIGHT / 2. - 45.;
    let w = WIDTH / 2.;
    let h = (HEIGHT / 2.) + 5.;

    // columns
    let c1 = x + 10.;
    let c2 = c1 + 100.;
    let c3 = c2 + 100.;
    let c4 = c3 + 100.;

    if SCHEDULE_ACTIVE {
        draw_rectangle(x, y, w, h, fade(DARKGRAY, 0.8)); // dark background
        draw_rectangle(c1, y + 21., w - 20., 4., GOLD); // gold bar
        draw_text("Flight #", c1, y + 15., 20.0, GOLD);
        draw_text("Arrival", c2, y + 15., 20.0, GOLD);
        draw_text("Departure", c3, y + 15., 20.0, GOLD);
        draw_text("Gate", c4, y + 15., 20.0, GOLD);

        if let Some(flights) = &FLIGHTS {
            for (i, f) in flights.iter().enumerate() {
                // iterate the flights and draw information for each flight incomplete flight
                match f.status {
                    FlightStatus::Done => {}
                    FlightStatus::Pre | FlightStatus::Air | FlightStatus::Apron(_) => {
                        let yi = y + (27. * (i as f32 + 1.5)) + 5.;
                        // flight num
                        draw_text(format!("{}", f.id).as_str(), c1, yi, 20.0, GOLD);
                        // plane sprite
                        match &PLANE_TEXTURES {
                            Some(textures) => {
                                draw_texture(textures[f.sprite as usize], c1 + 35., yi - 20., WHITE)
                            }
                            None => {}
                        };
                        // arrival
                        let a = minute_to_digits((f.arrival / MIN) as i32);
                        draw_text(
                            format!("{}{}:{}{}", a.0, a.1, a.2, a.3).as_str(),
                            c2,
                            yi,
                            20.0,
                            GOLD,
                        );
                        // departure
                        let d = minute_to_digits((f.departure / MIN) as i32);
                        draw_text(
                            format!("{}{}:{}{}", d.0, d.1, d.2, d.3).as_str(),
                            c3,
                            yi,
                            20.0,
                            GOLD,
                        );
                        // gate
                        draw_text(format!("{}", f.apron).as_str(), c4, yi, 20.0, GOLD);

                        // draw a highlighting rectangle if the mouse is over a flight
                        let xr = x + 5.;
                        let yr = yi - 17.;
                        let wr = w - 10.;
                        let hr = 25.;
                        if collision(mouse_position(), xr, yr, wr, hr) {
                            draw_rectangle_lines(xr, yr, wr, hr, 4.0, GOLD);
                        }
                    }
                }
            }
        }
    }
}
unsafe fn draw_clock() {
    //
    let x = WIDTH / 2.;
    let y = HEIGHT / 8.;
    let time = minute_to_digits((FRAME_COUNTER / MIN) as i32);
    let texture_p = DrawTextureParams {
        dest_size: Some(Vec2::new(16., 16.)),
        ..Default::default()
    };
    if let Some(odot) = ODOT_TEXTURE {
        draw_texture(odot, x, y, WHITE);
        draw_texture(odot, x, y - 6., WHITE);
    }
    if let Some(digits) = &DIGIT_TEXTURES {
        draw_texture_ex(
            digits[time.0 as usize],
            x - 32.,
            y,
            WHITE,
            texture_p.to_owned(),
        );
        draw_texture_ex(
            digits[time.1 as usize],
            x - 16.,
            y,
            WHITE,
            texture_p.to_owned(),
        );
        draw_texture_ex(
            digits[time.2 as usize],
            x + 16.,
            y,
            WHITE,
            texture_p.to_owned(),
        );
        draw_texture_ex(
            digits[time.3 as usize],
            x + 32.,
            y,
            WHITE,
            texture_p.to_owned(),
        );
    }
}
unsafe fn draw_score() {
    let x = WIDTH * 0.75;
    let y = HEIGHT * 0.10;
    let w = 75.;
    let h = 50.;
    draw_rectangle(x, y, w, h, fade(DARKGRAY, 0.8));
    draw_text(
        format!("Good {}", GOOD).as_str(),
        x + 5.,
        y + 15.,
        20.,
        GOLD,
    );
    draw_text(format!("Ok {}", OK).as_str(), x + 5., y + 30., 20., GOLD);
    draw_text(format!("Bad {}", BAD).as_str(), x + 5., y + 45., 20., GOLD);
}

unsafe fn draw_flights() {
    if let Some(flights) = &FLIGHTS {
        for f in flights {
            if let FlightStatus::Air = f.status {
                if let Some(planes) = &PLANE_TEXTURES {}
            }
        }
    }
}

fn test_flight_set() -> Vec<Flight> {
    let mut out: Vec<Flight> = Vec::new();
    out.push(Flight {
        id: 1,
        arrival: 75 * MIN,
        departure: (75 + 60) * MIN,
        apron: 0,
        status: FlightStatus::Pre,
        sprite: 0,
    });
    out.push(Flight {
        id: 2,
        arrival: 130 * MIN,
        departure: 190 * MIN,
        apron: 1,
        status: FlightStatus::Pre,
        sprite: 1,
    });
    out.push(Flight {
        id: 3,
        arrival: 230 * MIN,
        departure: 290 * MIN,
        apron: 2,
        status: FlightStatus::Pre,
        sprite: 2,
    });
    out
}
fn minute_to_digits(min: i32) -> (i32, i32, i32, i32) {
    let h = min / 60;
    let m = min - 60 * h;
    let a = h / 10;
    let b = h % 10;
    let c = m / 10;
    let d = m % 10;
    (a, b, c, d)
}
/*
unsafe fn arial_flights() -> Vec<Flight> {
    let out: Vec<Flight> = Vec::new();
    if let Some(flights) = FLIGHTS.iter() {
        // return flights.iter().filter(|f| f.status == FlightStatus::Air);
        for f in flights.iter() {
            if f.status == FlightStatus::Air {
                out.push(f);
            }
        }
    }
    out
}
*/
