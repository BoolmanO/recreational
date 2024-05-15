use crate::air::util::effect::DefaultEffect;

fn init_vec(count: usize, default: Option<f64>, step: Option<f64>) -> Vec<f64> {
    let mut raw = vec![];
    let d = match default {
        None => 0.0,
        Some(v) => v,
    };
    let v_step = match step {
        None => 0.0,
        Some(v) => v,
    };
    for i in 0..count {
        raw.push(d + (v_step * i as f64));
    }
    raw
}
#[macro_export]
macro_rules! print_mat {
    ($mat_name:ident) => {
        print!("{}: [\n", stringify!($mat_name));
        let mut st = 0;
        for i in 0..$mat_name.as_raw().len() {
            if st == $mat_name.size().1 {
                st = 0;
                println!();
            }

            st += 1;
            print!("{:5}|", $mat_name.as_raw()[i]);
        }
        println!("\n]");
    };
}

pub struct Mat {
    rows: usize,
    cols: usize,
    raw: Vec<f64>,
}

impl Mat {
    pub fn new(rows: usize, cols: usize) -> Self {
        let raw = init_vec(rows * cols, None, None);
        Mat { rows, cols, raw }
    }
    pub fn newd(rows: usize, cols: usize, default: f64) -> Self {
        let raw = init_vec(rows * cols, Some(default), None);
        Mat { rows, cols, raw }
    }
    pub fn newr(rows: usize, cols: usize, default: f64, step: f64) -> Self {
        let raw = init_vec(rows * cols, Some(default), Some(step));
        Mat { rows, cols, raw }
    }
    pub fn newde(rows: usize, cols: usize, deffect: &mut dyn DefaultEffect) -> Self {
        let mut raw = vec![];
        for _ in 0..rows * cols {
            raw.push(deffect.cb());
        }
        Mat { rows, cols, raw }
    }
    pub fn size(&self) -> (usize, usize) {
        (self.rows, self.cols)
    }

    pub fn as_raw(&self) -> Vec<f64> {
        self.raw.clone()
    }
    pub fn at(&self, row_index: usize, col_index: usize) -> f64 {
        return self.raw[(row_index * self.cols) + (col_index)];
    }

    pub fn from_vec(rows: usize, cols: usize, origin_vec: Vec<f64>) -> Self {
        if rows * cols != origin_vec.len() {
            panic!(
                "Attemp to initialize matrix from vec that length are invalid. Should be {}, but actually is {}", 
                rows*cols, origin_vec.len()
            );
        }
        return Mat {
            rows,
            cols,
            raw: origin_vec,
        };
    }

    pub fn sum(&self, another: &Mat) -> Mat {
        if self.rows != another.rows || self.cols != another.cols {
            panic!("Attemp to sum matrixes when sizes are different");
        }
        if self.raw.len() != another.raw.len() {
            panic!("Attemp to sum matrixes when lengths are different");
        }
        let mut raw = vec![];
        for i in 0..self.raw.len() {
            raw.push(self.raw[i] + another.raw[i]);
        }
        Mat::from_vec(self.rows, self.cols, raw)
    }
    pub fn mul_by(&self, another: &Mat) -> Mat {
        if self.cols != another.rows {
            panic!("Matrixes multiplying rules are violated. Origin matrix columns count should be equivalent to second matrix rows count, but now {} columns and {} rows", self.cols, self.rows);
        }
        let self_m = &self.raw;
        let an_m = &another.raw;
        let mut raw = vec![];
        for i in 0..self.rows {
            for j in 0..another.cols {
                let mut calc: f64 = 0.0;
                for k in 0..self.cols {
                    calc += self_m[i * self.cols + k] * an_m[k * another.cols + j];
                }
                raw.push(calc);
            }
        }
        Mat::from_vec(self.rows, another.cols, raw)
    }
    pub fn transposition(&self) -> Mat {
        todo!();
        // i think thats useless in context of ML.
    }
}
