#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <limits>
#include <algorithm>

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

/*    similar with std::transform_n       */
template<int N,class Op, class Out, class In_1, class...In_N>
inline auto transform_N(
    Op op,
    Out out,
    In_1 in_1,
    In_N...in_n) noexcept
{
   for(int i=0;i<N;i++)
      *out++ = op(*in_1++, *in_n++...);
    return out;
}
void poisson2d(int &iteration)
{
    constexpr int M = 100;
    constexpr double epsilon0 = 8.85e-12;
    constexpr double target = 1.0e-12;
    constexpr double a = 0.01;
    constexpr double a2 = a*a/epsilon0;
    constexpr double a3=0.25;
    double phi[M*M]={};
    double phiprime[M*M]={};
    double rhoarr[M*M]={};
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < M; ++j)
            rhoarr[i*M+j] = rho(i * a, j * a);
    }
    int iter = 0;
    for(bool b=true;b;std::copy_n(phiprime, M*M, phi))
    {
        iter += 1;
        for (int i = 1; i < M - 1; ++i)
            transform_N<M-2>([](auto u,auto l,auto r,auto x,auto m){
                return a3*(u+l+r+x+a2*m);
            }, phiprime+i*M+1, phi+(i+1)*M+1, phi+(i-1)*M+1,phi+i*M+2,phi+i*M,rhoarr+i*M+1);
        b=std::mismatch(phi, phi+M*M, phiprime, [](auto l,auto r){return (r-l)*(r-l)<target;}).first!=(phi+M*M);
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

