# About this repository

This repository uses Microsoft SEAL (CKKS Scheme) to implement matrix multiplication and convolution. It aims to measure execution time of major calculations in Homomorphic Encryption (HE) and works as a benchmark.

Half of programs in this repository supports parallel execution with OpenMP. 

The programs in this repository is listed below.

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

