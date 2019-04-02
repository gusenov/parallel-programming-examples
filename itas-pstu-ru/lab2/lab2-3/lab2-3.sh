#!/bin/bash

rm lab2-3.out
mpicc -o lab2-3.out lab2-3.c
mpiexec -np 6 ./lab2-3.out
