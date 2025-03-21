Please refer to README-English.md for a description in English.

# リポジトリの目的
ここでは、完全準同型暗号のライブラリMicrosort SEALを使用して、行列積と畳み込みを計算するプログラムを実装した。これらのプログラムはOpenMPを使用して並列実行することが可能である。

各ディレクトリに配置されているプログラムの機能は、以下のようになっている。

| 番号　| 実行する演算 | 暗号化 | OpenMPによる並列化 |
| --- | ------------- | ------------- | ---- |
| 1 | 行列積 | x | x |
| 2 | 行列積 | x | o |
| 3 | 行列積 | o (CKKS方式) | x |
| 4 | 行列積 | o (CKKS方式) | o |
| 5 | 畳み込み | x | x |
| 6 | 畳み込み | x | o |
| 7 | 畳み込み | o (CKKS方式) | x |
| 8 | 畳み込み | o (CKKS方式) | o |

# 動作環境
以下の環境で動作確認をしている。

| 機種　| OS | コンパイラなど | CPU |
| --- | ------------- | ------------- | ---- |
| サーバ | Ubuntu 22.04.3 LTS | CMake 3.22.1 + GNU GCC 11.4.0 | Intel Xeon Silver 4316 |
| Raspberry Pi 5 | Debian 12.8 | CMake 3.25.1 + GNU GCC 12.2.0 | Arm Cortex A76 |
| MacBook Pro | macOS 15.2 | CMake 3.31.3 + LLVM Clang 19.1.6 | Apple M3 Pro |

# 実行環境構築
サーバの場合: 
以下のコマンドを実行する。
```
mkdir seal_install
git clone https://github.com/microsoft/SEAL.git
cd SEAL
cmake -S . -B build -DSEAL_BUILD_EXAMPLES=ON -DSEAL_BUILD_TESTS=ON -DSEAL_BUILD_BENCH=ON -DCMAKE_INSTALL_PREFIX=~/seal_install
cmake --build build
sudo cmake --install build
```

テストは以下のコマンドを実行して行う。
```
cd ~/SEAL/build/bin
./sealtest
```

MacBook Proの場合:
1. ターミナルを開き、cmakeとXcode toolchainが入っているかを確認する。ない場合はインストールする。
2. Microsoft SEALをインストールするためのディレクトリ(例としてms_seal)を作成し、以下を実行する。
```
cd ms_seal
git clone https://github.com/microsoft/SEAL.git
cmake -S . -B build
cmake --build build
sudo cmake --install build
```

テストとしてexamplesを実行する場合は、以下のコマンドを実行する。
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
