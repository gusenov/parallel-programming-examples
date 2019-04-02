#!/bin/bash

rm lab5-2.out
mpicc -o lab5-2.out lab5-2.c -lg2 -lm -lX11
mpiexec -np 8 ./lab5-2.out
