#!/usr/bin/env bash

readonly PATH_TO_MARL="marl"

[ -d "$PATH_TO_MARL" ] && rm -rf "$PATH_TO_MARL"

git clone https://github.com/google/marl.git --depth=1

if [ ! -d "$PATH_TO_MARL" ]; then
    exit
fi

# The first time pushd dir is called, pushd pushes the current directory onto the stack, then cds to dir and pushes it onto the stack.
pushd "$PATH_TO_MARL"

mkdir build

# Subsequent calls to pushd dir cd to dir and push dir only onto the stack.
pushd build

cmake .. -DMARL_BUILD_EXAMPLES=1 -DMARL_BUILD_TESTS=1
make

# popd removes the top directory off the stack, revealing a new top. Then it cds to the new top directory.
popd

find . -mindepth 1 -maxdepth 1 ! -iname "include" ! -iname "build" -exec rm -rf {} \;
find "./build" -mindepth 1 -maxdepth 1 ! -iname "libmarl.a" -exec rm -rf {} \;
