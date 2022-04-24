#!/bin/bash

# compile
gfortran test_cases.f90 -Ofast -o fortran_benchmark
g++ test_cases.cpp -Ofast -o cpp_benchmark

# run
./fortran_benchmark | tee fortran_results.txt
./cpp_benchmark | tee cpp_results.txt

OS=$(lsb_release -d | cut -f2 -d:)
CPU=$(cat /proc/cpuinfo | grep name | cut -f2 -d: | uniq)

# draw
python3 plot_to_html.py ${GITHUB_SHA::8} $OS $CPU

# deploy
if [ ! -d "/page" ]; then
  mkdir ./page
fi
mv index.html ./page/index.html
