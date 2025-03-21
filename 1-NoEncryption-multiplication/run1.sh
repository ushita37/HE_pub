#!/bin/bash

echo "1-NoEncryption-multiplication: sequential"

array=(2 4 8 16 32 64 128)
for matrixSize in "${array[@]}"; do
    echo "Matrix size: $matrixSize x $matrixSize"
done