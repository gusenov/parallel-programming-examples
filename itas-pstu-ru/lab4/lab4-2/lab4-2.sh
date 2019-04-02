#!/bin/bash

rm lab4-2.out
mpicc -o lab4-2.out lab4-2.c
mpiexec -np 5 ./lab4-2.out
