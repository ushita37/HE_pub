#!/bin/bash
rm -rf bulid
cmake -S . -B build -DCMAKE_PREFIX_PATH=~/seal_install_dir # if you have installed SEAL globally, you should remove the "-DCMAKE_PREFIX_PATH=~/seal_install_dir"
cmake --build build