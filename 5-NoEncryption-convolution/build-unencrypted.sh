#!/bin/bash
rm -rf bulid
cmake -S . -B build
cmake --build build