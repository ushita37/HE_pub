# About this repository

This repository uses Microsoft SEAL (CKKS Scheme) to implement matrix multiplication and convolution. It aims to measure the execution time of major calculations in Homomorphic Encryption (HE) and works as a benchmark.

Half of the programs in this repository support parallel execution with OpenMP.

The programs in this repository are listed below.

| Number　| kind of calcuration | encryption | parallel execution |
| --- | ------------- | ------------- | ---- |
| 1 | matrix mult. | x | x |
| 2 | matrix mult. | x | o |
| 3 | matrix mult. | o (CKKSscheme) | x |
| 4 | matrix mult. | o (CKKSscheme) | o |
| 5 | convolution | x | x |
| 6 | convolution | x | o |
| 7 | convolution | o (CKKSscheme) | x |
| 8 | convolution | o (CKKSscheme) | o |


# Execution Environment
This software has been tested in the following environment.

| Device　| OS | Compiler | CPU |
| --- | ------------- | ------------- | ---- |
| Server | Ubuntu 22.04.3 LTS | CMake 3.22.1 + GNU GCC 11.4.0 | Intel Xeon Silver 4316 |
| Raspberry Pi 5 | Debian 12.8 | CMake 3.25.1 + GNU GCC 12.2.0 | Arm Cortex A76 |
| MacBook Pro | macOS 15.2 | CMake 3.31.3 + LLVM Clang 19.1.6 | Apple M3 Pro |
