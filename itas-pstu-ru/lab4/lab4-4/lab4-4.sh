#!/bin/bash

rm lab4-4.out
mpicc -o lab4-4.out lab4-4.c
mpiexec -np 8 ./lab4-4.out
