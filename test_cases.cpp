#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <chrono>
#include <limits>

template <size_t M, size_t N>
using mat = std::array<std::array<double, N>, M>;

void print_benchmark_result(const std::string &name, const double time)
{
    std::cout << "Cpp," << name << ',' << std::setprecision(6) << time << std::endl;
}

template <size_t M, size_t N>
void swap(mat<M, N> &a, const mat<M, N> &b)
{
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
            a[i][j] = b[i][j];
    }
}

template <size_t M, size_t N>
double mmax(const mat<M, N> &a, const mat<M, N> &b)
{
    double max = 0.0;
    double diff = 0.0;
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            diff = std::abs(a[i][j] - b[i][j]);
            if (diff > max)
                max = diff;
        }
    }
    return max;
}

double rho(double x, double y)
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
    constexpr double a = 0.01;

    mat<M, M> phi{};
    mat<M, M> phiprime{};
    mat<M, M> rhoarr;

    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < M; ++j)
            rhoarr[i][j] = rho(i * a, j * a);
    }
    double delta = 1.0;
    int iter = 0;
    double a2 = a * a;
    while (delta > target)
    {
        iter += 1;
        for (int i = 1; i < M - 1; ++i)
        {
            for (int j = 1; j < M - 1; ++j)
            {
                phiprime[i][j] = (phi[i + 1][j] + phi[i - 1][j] + phi[i][j + 1] + phi[i][j - 1] + a2 * rhoarr[i][j] / epsilon0) / 4.0;
            }
        }
        delta = mmax(phi, phiprime);
        swap(phi, phiprime);
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