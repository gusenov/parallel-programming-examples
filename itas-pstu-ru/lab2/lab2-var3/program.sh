#!/bin/bash

rm program.out
mpicc -o program.out program.c
mpiexec -np 10 ./program.out
