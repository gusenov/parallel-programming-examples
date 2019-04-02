#!/bin/bash

rm lab2-2.out
mpicc -o lab2-2.out lab2-2.c
mpiexec -np 2 ./lab2-2.out
