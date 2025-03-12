#include <iostream>
#include <vector>
#include <chrono>
#include "matrix.hpp"

// 畳み込み計算をするプログラム
// パラメータINPUT_SIZE_ROWS, INPUT_SIZE_COLS, KERNEL_SIZE_ROWS, KERNEL_SIZE_COLSを変更して実行する
bool print_matrices = false; // 行列を出力するかのフラグ 変更可

void print_matrix(const std::vector<std::vector<double>>& matrix) {
    for (const auto& row : matrix) {
        for (double val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}


int main(int argc, char *argv[]) {
    int INPUT_SIZE_ROWS, INPUT_SIZE_COLS, KERNEL_SIZE_ROWS, KERNEL_SIZE_COLS;
    INPUT_SIZE_ROWS = INPUT_SIZE_COLS = std::stoi(argv[1]); // 入力の行数、列数
    KERNEL_SIZE_ROWS = KERNEL_SIZE_COLS = std::stoi(argv[2]);   // カーネルの行数、列数

    // 行列input, kernelの生成
    int external_seed_A = 0;
    int external_seed_B = 1;

    auto input = generate_matrix(INPUT_SIZE_ROWS, INPUT_SIZE_COLS, external_seed_A);
    auto kernel = generate_matrix(KERNEL_SIZE_ROWS, KERNEL_SIZE_COLS, external_seed_B);

    // 畳み込みの計算
    auto start_mul = std::chrono::high_resolution_clock::now();
    auto result = convolveDirect(input, kernel);
    auto end_mul = std::chrono::high_resolution_clock::now();

    // 時間の計測
    auto duration_mul = std::chrono::duration_cast<std::chrono::microseconds>(end_mul - start_mul).count();

    std::cout << "Matrix multiplication time: " << duration_mul << " microseconds" << std::endl;

    return 0;
}
