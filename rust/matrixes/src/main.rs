mod air;

use crate::air::prelude::*;
use crate::air::util::prelude::DefaultEffect;

struct MyEffect {
    c: i64,
}
impl DefaultEffect for MyEffect {
    fn cb(&mut self) -> f64 {
        self.c += 1;
        return self.c as f64;
    }
}

fn main() {
    let m1 = Mat::newr(2, 3, 1.0, 1.0);
    let m2 = Mat::newr(3, 6, 7.0, 1.0);
    let m3 = m1.mul_by(&m2);
    let mut e = MyEffect { c: 0 };
    let m4 = Mat::newde(m3.size().1, m3.size().0, &mut e);
    print_mat!(m4);
    let m5 = m3.mul_by(&m4);
    print_mat!(m5);
}
