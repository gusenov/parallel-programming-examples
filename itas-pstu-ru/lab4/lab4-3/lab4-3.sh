#!/bin/bash

rm lab4-3.out
mpicc -o lab4-3.out lab4-3.c
mpiexec -np 5 ./lab4-3.out
