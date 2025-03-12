cmake_minimum_required(VERSION 3.10)
project(MySEALProject)

set(CMAKE_CXX_STANDARD 17)

# 実行ファイルを追加 (ターゲットを明示)
add_executable(main main.cpp matrix.cpp)

# コンパイルオプションの追加
target_compile_options(main PUBLIC -O2 -Wall -DNDEBUG)