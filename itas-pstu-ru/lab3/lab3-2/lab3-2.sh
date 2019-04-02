#!/bin/bash

rm lab3-2.out
mpicc -o lab3-2.out lab3-2.c
mpiexec -np 3 ./lab3-2.out
