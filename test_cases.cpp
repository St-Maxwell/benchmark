#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <chrono>
#include <limits>
#include <algorithm>
template <size_t M>
using mat = std::array<double, M*M>;

void print_benchmark_result(const std::string &name, const double time)
{
    std::cout << "Cpp," << name << ',' << std::setprecision(6) << time << std::endl;
}

inline double rho(double x, double y) noexcept
{
    constexpr double s1 = 0.6;
    constexpr double e1 = 0.8;
    constexpr double s2 = 0.2;
    constexpr double e2 = 0.4;

    if (x > s1 && x < e1 && y > s1 && y < e1)
    {
        return 1.0;
    }
    else if (x > s2 && x < e2 && y > s2 && y < e2)
    {
        return -1.0;
    }
    else
    {
        return 0.0;
    }
}

void poisson2d(int &iteration)
{
    constexpr int M = 100;
    constexpr double epsilon0 = 8.85e-12;
    constexpr double target = 1.0e-6;
    constexpr double target1= -target;
    constexpr double a = 0.01;
    constexpr double a2 = a * a/epsilon0;
    constexpr double a3=0.25;
    mat<M> phi{};
    mat<M> phiprime{};
    mat<M> rhoarr {};

    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < M; ++j)
            rhoarr[i*M+j] = rho(i * a, j * a);
    }
    double delta = 1.0;
    int iter = 0;
    for(bool b=true;b;)
    {
        iter += 1;
        for (int i = 1; i < M - 1; ++i) {
            for (int j = 1; j < M - 1; ++j) {
                auto u = phi[(i - 1) * M + j];
                auto d = phi[(i + 1) * M + j];
                auto l = phi[i * M + j - 1];
                auto r = phi[i * M + j + 1];
                auto o = rhoarr[i * M + j];
                phiprime[i * M + j] = ((a * a / epsilon0) * o + (u + d + l + r)) * 0.25;
            }
        }
        b=std::mismatch(phi.begin(), phi.end(), phiprime.begin(), [](auto l,auto r){auto t=r-l;return target1<t&&t<target;}).first!=phi.end();
        std::copy_n(phiprime.begin(), M*M, phi.begin());
    }
    iteration = iter;
}

int main()
{
    constexpr int NRUNS = 20;
    double tmin = std::numeric_limits<double>::max();
    int iter;

    for (int i = 0; i < 5; ++i)
    {
        auto t1 = std::chrono::system_clock::now();
        for (int j = 0; j < NRUNS; ++j)
            poisson2d(iter);
        auto t2 = std::chrono::system_clock::now();
        std::chrono::duration<double> duration = t2 - t1;
        if (duration.count() < tmin)
            tmin = duration.count();
    }

    print_benchmark_result("poisson2d", tmin / NRUNS);

    return 0;
}

