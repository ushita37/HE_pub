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

# Environment Setup
In the case of the server: 
Execute the following commands.
```
mkdir seal_install
git clone https://github.com/microsoft/SEAL.git
cd SEAL
cmake -S . -B build -DSEAL_BUILD_EXAMPLES=ON -DSEAL_BUILD_TESTS=ON -DSEAL_BUILD_BENCH=ON -DCMAKE_INSTALL_PREFIX=~/seal_install
cmake --build build
sudo cmake --install build
```

The test is performed by executing the following command.
```
cd ~/SEAL/build/bin
./sealtest
```

In the case of MacBook Pro:
1. Open a terminal and check if the CMake and the Xcode toolchain are included. If not, install them.
2. Create a directory to install Microsoft SEAL (e.g. ms_seal) and execute the following.
```
cd ms_seal
git clone https://github.com/microsoft/SEAL.git
cmake -S . -B build
cmake --build build
sudo cmake --install build
```

To run examples as a test, execute the following command.
```
cmake -S . -B build -DSEAL_BUILD_EXAMPLES=ON
cmake --build build
./sealexamples
```

# 実行方法
それぞれのプログラムに対応するディレクトリに移動した後、以下のコマンドでビルドする。
Microsoft SEALをグローバルにインストールした場合
```
cmake -S . -B build
cmake --build build
```


Microsoft SEALを特定のディレクトリ`~/seal_install`にインストールした場合
```
cmake -S . -B build -DCMAKE_PREFIX_PATH=~/seal_install
cmake --build build
```


ビルドが完了したら、以下のコマンドで実行する。
行列積のプログラムを実行する場合（例えば、2つの行列のサイズを32×32とする場合）
```
./build/main 32 32 32
```


畳み込みのプログラムを実行する場合（例えば、入力行列のサイズが32×32、カーネルのサイズが16×16の場合）
```
./build/main 32 16
```
