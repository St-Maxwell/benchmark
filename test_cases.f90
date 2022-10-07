module definition
    use iso_fortran_env, only: real64, output_unit
    implicit none
    private
    public :: dp, init_random_seed, print_benchmark_result
    integer, parameter :: dp = real64
contains
    subroutine init_random_seed()
        integer :: i, n, clock
        integer, allocatable :: seed(:)
        call random_seed(size=n)
        allocate (seed(n))
        call system_clock(count=clock)
        seed = clock + 37*[(i - 1, i=1, n)]
        call random_seed(put=seed)
    end subroutine

    subroutine print_benchmark_result(name, duration)
        character(len=*), intent(in) :: name
        real(kind=dp), intent(in) :: duration ! unit: s
        write (output_unit, "('fortran,',A,',',f10.6)") name, duration
    end subroutine print_benchmark_result
end module definition

module tests
    use definition, only: dp
    implicit none

contains

    subroutine poisson2d(iteration)
        integer, intent(out) :: iteration
        integer, parameter :: M = 100
        real(dp), parameter :: epsilon0 = 8.85e-12_dp, target = 1.e-6_dp, a = 0.01_dp
        real(dp) :: phi(M, M), phiprime(M, M), rhoarr(M, M)
        real(dp) :: a2
        integer :: i, j, iter

        phi(:, :) = 0._dp
        phiprime(:, :) = 0._dp
        do j = 1, M
            do i = 1, M
                rhoarr(i, j) = rho((i-1)*a, (j-1)*a)
            end do
        end do
        iter = 0
        a2 = a*a
        do
            iter = iter + 1
            do j = 2, M - 1
                do i = 2, M - 1
                    phiprime(i, j) = (phi(i + 1, j) + phi(i - 1, j) + phi(i, j + 1) + phi(i, j - 1) &
                                      + a2*rhoarr(i, j)/epsilon0)/4.0_dp
                end do
            end do
            if(ismin())exit
            phi = phiprime
        end do
        iteration = iter
    contains
       pure logical function ismin()result(res)
          integer::i,j
          do j=2,m-1
             do i=2,m-1
                  associate(x=>phiprime(i,j)-phi(i,j))
                     res= x > -target .and. x <target
                     if(.not.res)return
                  end associate
             end do
          end do
       end function ismin

        pure function rho(x, y)
            real(dp), intent(in) :: x, y
            real(dp) :: rho
            if (x > 0.6_dp .and. x < 0.8_dp .and. y > 0.6_dp .and. y < 0.8_dp) then
                rho = 1.0_dp
            else if (x > 0.2_dp .and. x < 0.4_dp .and. y > 0.2_dp .and. y < 0.4_dp) then
                rho = -1.0_dp
            else
                rho = 0.0_dp
            end if
        end function rho
    end subroutine poisson2d

end module tests

program main
    use definition
    use tests
    implicit none
    integer, parameter :: NRUNS = 20
    real(dp) :: t1, t2, tmin
    integer :: i, j
    integer :: iter

    call init_random_seed()

    tmin = huge(0._dp)
    do i = 1, 5
        call cpu_time(t1)
        do j = 1, NRUNS
            call poisson2d(iter)
        end do
        call cpu_time(t2)
        if (t2 - t1 < tmin) tmin = t2 - t1
    end do
    call print_benchmark_result("poisson2d", tmin/NRUNS)

end program main
