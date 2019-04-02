#!/bin/bash

rm program.out
mpicc -o program.out program.c
mpiexec -np 3 ./program.out
