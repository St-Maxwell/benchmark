#!/bin/bash

# compile
gfortran test_cases.f90 -Ofast -o fortran_benchmark
g++ test_cases.cpp -Ofast -o cpp_benchmark

# run
./fortran_benchmark | tee fortran_results.txt
./cpp_benchmark | tee cpp_results.txt

# draw
python3 plot_to_html.py

# deploy
if [ ! -d "/page" ]; then
  mkdir ./page
fi
mv index.html ./page/index.html
