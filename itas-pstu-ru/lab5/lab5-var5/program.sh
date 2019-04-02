#!/bin/bash

rm program.out
mpicc -o program.out program.c -lg2 -lm -lX11
mpiexec -np 4 ./program.out
