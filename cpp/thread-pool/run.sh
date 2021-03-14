#!/usr/bin/env bash

EXE_FILE="./a.out"

[ -f "$EXE_FILE" ] && rm "$EXE_FILE"

g++ -std=gnu++17 -Wall main.cpp -lpthread -o "$EXE_FILE" && "$EXE_FILE"
