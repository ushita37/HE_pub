cmake_minimum_required(VERSION 3.13)
project(SimpleProduct)

set(SEAL_DIR ../SEAL/native)
set(CMAKE_CXX_STANDARD 17)

# SEALのライブラリを追加
find_package(SEAL 4.1.2 REQUIRED PATHS ${SEAL_DIR})

# 実行ファイルを追加 (ターゲットを明示)
add_executable(main main.cpp matrix.cpp)

# コンパイルオプションの追加
target_compile_options(main PUBLIC -O2 -Wall -DNDEBUG)

# SEALライブラリをリンク
# 2024.12.8 ウシタ SEAL::seal_sharedからSEAL::sealに変更
target_link_libraries(main SEAL::seal)

# For OpenMP
find_package(OpenMP REQUIRED)
if(OpenMP_FOUND)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


# コンパイラのバージョン確認
message(STATUS "\n【コンパイラの情報】\nC compiler: ${CMAKE_C_COMPILER}")
message(STATUS "\nCXX compiler: ${CMAKE_CXX_COMPILER}")
execute_process(COMMAND ${CMAKE_C_COMPILER} --version OUTPUT_VARIABLE C_COMPILER_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${CMAKE_CXX_COMPILER} --version OUTPUT_VARIABLE CXX_COMPILER_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "C compiler version: ${C_COMPILER_VERSION}")
message(STATUS "CXX compiler version: ${CXX_COMPILER_VERSION}\n\n")
