cmake_minimum_required(VERSION 3.10)
project(MySEALProject)

set(CMAKE_CXX_STANDARD 17)

# 実行ファイルを追加 (ターゲットを明示)
add_executable(main main.cpp matrix.cpp)

# コンパイルオプションの追加
target_compile_options(main PUBLIC -O2 -Wall -DNDEBUG)

# コンパイラのバージョン確認
message(STATUS "\n【コンパイラの情報】\nC compiler: ${CMAKE_C_COMPILER}")
message(STATUS "\nCXX compiler: ${CMAKE_CXX_COMPILER}")
execute_process(COMMAND ${CMAKE_C_COMPILER} --version OUTPUT_VARIABLE C_COMPILER_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${CMAKE_CXX_COMPILER} --version OUTPUT_VARIABLE CXX_COMPILER_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "C compiler version: ${C_COMPILER_VERSION}")
message(STATUS "CXX compiler version: ${CXX_COMPILER_VERSION}\n\n")
