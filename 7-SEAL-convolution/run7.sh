#!/bin/bash

echo "7-SEAL-convolution: sequential"

array=(2 4 8 16 32 64)
for inputN in "${array[@]}"; do
    for kernelN in "${array[@]}"; do
        if [ $inputN -ge $kernelN ]; then
            echo "Input size: $inputN x $inputN, Kernel size: $kernelN x $kernelN"
            /build/main $inputN $kernelN
        fi
    done
done