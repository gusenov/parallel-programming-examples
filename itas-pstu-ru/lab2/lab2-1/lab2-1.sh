#!/bin/bash

rm lab2-1.out
mpicc -o lab2-1.out lab2-1.c
mpiexec -np 10 ./lab2-1.out
