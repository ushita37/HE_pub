#!/bin/bash

echo "2-OpenMP-NoEncryption-multiplication: parallel"

array=(2 4 8 16 32 64 128)
for matrixSize in "${array[@]}"; do
    echo "Matrix size: $matrixSize x $matrixSize"
done