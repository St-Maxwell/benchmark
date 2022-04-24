use std::time::Instant;

fn print_benchmark_result(name: &String, time: f64) {
    println!("rust,{},{:.6}", name, time);
}

fn rho(x: f64, y: f64) -> f64 {
    if x > 0.6 && x < 0.8 && y > 0.6 && y < 0.8 {
        1.0
    } else if x > 0.2 && x < 0.4 && y > 0.2 && y < 0.4 {
        -1.0
    } else {
        0.0
    }
}

fn swap<const M: usize, const N: usize>(a: &mut [[f64; N]; M], b: &[[f64; N]; M]) {
    for i in 0..M {
        for j in 0..N {
            a[i][j] = b[i][j];
        }
    }
}

fn mmax<const M: usize, const N: usize>(a: &[[f64; N]; M], b: &[[f64; N]; M]) -> f64 {
    let mut max = 0.0;
    for i in 0..M {
        for j in 0..N {
            let diff = (a[i][j] - b[i][j]).abs();
            if diff > max {
                max = diff;
            }
        }
    }
    max
}

fn poisson2d(iteration: &mut i32) {
    const M: usize = 100;
    let epsilon0 = 8.85e-12;
    let target = 1.0e-6;
    let a = 0.01;

    let mut phi = [[0.0; M]; M];
    let mut phiprime = [[0.0; M]; M];
    let mut rhoarr = [[0.0; M]; M];

    for i in 0..M {
        for j in 0..M {
            rhoarr[i][j] = rho(a * i as f64, a * j as f64);
        }
    }
    let mut delta: f64 = 1.0;
    let mut iter = 0;
    let a2 = a * a;
    while delta > target {
        iter += 1;
        for i in 1..M - 1 {
            for j in 1..M - 1 {
                phiprime[i][j] = (phi[i + 1][j]
                    + phi[i - 1][j]
                    + phi[i][j + 1]
                    + phi[i][j - 1]
                    + a2 * rhoarr[i][j] / epsilon0)
                    / 4.0
            }
        }
        delta = mmax(&phi, &phiprime);
        swap(&mut phi, &phiprime);
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
