#!/bin/bash

rm lab3-1.out
mpicc -o lab3-1.out lab3-1.c
mpiexec -np 2 ./lab3-1.out
