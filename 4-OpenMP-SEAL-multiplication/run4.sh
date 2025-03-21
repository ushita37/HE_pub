#!/bin/bash

echo "4-OpenMP-SEAL-multiplication: parallel"

array=(2 4 8 16 32 64 128)
for matrixSize in "${array[@]}"; do
    echo "Matrix size: $matrixSize x $matrixSize"
done