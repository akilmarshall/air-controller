pub fn collision(p: (f32, f32), x: f32, y: f32, w: f32, h: f32) -> bool {
    // is the point p  in the rectangle (x, y, w, h) ?
    x < p.0 && p.0 < x + w && y < p.1 && p.1 < y + h
}
