#!/bin/bash

clear

rm *.out
gcc -Wall -pthread phil.c -o phil.out && ./phil.out

