FC=gfortran
CC=gcc

bin/fbin: hello.f90
	gfortran hello.f90 -o $@

bin/cbin: hello.c
	gcc hello.c -o $@

perform_benchmark: bin/fbin bin/cbin
	./bin/fbin
	./bin/cbin
