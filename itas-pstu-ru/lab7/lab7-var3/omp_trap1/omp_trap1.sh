#!/bin/bash

rm omp_trap1.out
gcc -g -Wall -fopenmp -o omp_trap1.out omp_trap1.c -lm && ./omp_trap1.out
