#!/bin/bash

rm lab4-1.out
mpicc -o lab4-1.out lab4-1.c
mpiexec -np 5 ./lab4-1.out
