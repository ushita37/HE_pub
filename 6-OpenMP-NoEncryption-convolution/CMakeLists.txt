cmake_minimum_required(VERSION 3.10)
project(MySEALProject)

set(CMAKE_CXX_STANDARD 17)

# 実行ファイルを追加 (ターゲットを明示)
add_executable(main main.cpp matrix.cpp)

# コンパイルオプションの追加
target_compile_options(main PUBLIC -O2 -Wall -DNDEBUG)

# For OpenMP
find_package(OpenMP REQUIRED)
if(OpenMP_FOUND)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()