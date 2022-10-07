use std::mem::swap;
use std::time::Instant;

fn print_benchmark_result(name: &String, time: f64) {
    println!("rust,{},{:.6}", name, time);
}

#[inline(always)]
fn rho(x: f64, y: f64) -> f64 {
    if x > 0.6 && x < 0.8 && y > 0.6 && y < 0.8 {
        1.0
    } else if x > 0.2 && x < 0.4 && y > 0.2 && y < 0.4 {
        -1.0
    } else {
        0.0
    }
}

fn poisson2d(iteration: &mut i32) {
    const M: usize = 100;
    let eps = 8.85e-12;
    let target = (-1.0e-6, 1.0e-6);
    let a = 0.01;

    let mut phi = &mut [[0.0; M]; M];
    let mut phiprime = &mut [[0.0; M]; M];
    let rhoarr = &mut [[0.0; M]; M];

    for i in 0..M {
        for j in 0..M {
            rhoarr[i][j] = rho(a * i as f64, a * j as f64);
        }
    }

    let mut iter = 0;
    'iter: loop {
        iter += 1;
        for i in 1..M - 1 {
            for j in 1..M - 1 {
                let u = phi[i - 1][j];
                let d = phi[i + 1][j];
                let l = phi[i][j - 1];
                let r = phi[i][j + 1];
                let o = rhoarr[i][j];
                phiprime[i][j] = ((u + d + l + r) + (a * a / eps) * o) * 0.25;
            }
        }
        for i in 0..M {
            for j in 0..M {
                let t = phiprime[i][j] - phi[i][j];
                if !(target.0 < t && t < target.1) {
                    swap(&mut phi, &mut phiprime);
                    continue 'iter;
                }
            }
        }
        break;
    }
    *iteration = iter;
}

fn main() {
    const NRUNS: i32 = 20;
    let mut tmin = std::f64::MAX;
    let mut iter = 0;

    for _ in 0..5 {
        let start = Instant::now();
        for _ in 0..NRUNS {
            poisson2d(&mut iter);
        }
        let duration = start.elapsed();
        if duration.as_secs_f64() < tmin {
            tmin = duration.as_secs_f64();
        }
    }
    print_benchmark_result(&String::from("poisson2d"), tmin / NRUNS as f64);
}
