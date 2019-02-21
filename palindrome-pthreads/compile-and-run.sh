#!/bin/bash

clear

rm *.out
gcc -Wall -pthread program.c -o program.out && ./program.out
