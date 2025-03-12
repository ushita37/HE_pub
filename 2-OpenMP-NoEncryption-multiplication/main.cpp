#include <iostream>
#include <vector>
#include <chrono>
#include "matrix.hpp"

bool print_matrices = false; 

void print_matrix(const std::vector<std::vector<double> >& matrix) {
    for (const auto& row : matrix) {
        for (double val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char *argv[]) {
    int MATRIX_SIZE_A_ROWS, MATRIX_SIZE_A_COLS, MATRIX_SIZE_B_COLS;
    MATRIX_SIZE_A_ROWS = std::stoi(argv[1]); // 行列サイズ
    MATRIX_SIZE_A_COLS = std::stoi(argv[2]);
    MATRIX_SIZE_B_COLS = std::stoi(argv[3]);
    int external_seed_A = 0;
    int external_seed_B = 1;
  
    auto A = generate_matrix(MATRIX_SIZE_A_ROWS, MATRIX_SIZE_A_COLS, external_seed_A);
    auto B = generate_matrix(MATRIX_SIZE_A_COLS, MATRIX_SIZE_B_COLS, external_seed_B);
  
    auto start_mul = std::chrono::high_resolution_clock::now();
    auto C = multiply_matrices(A, B);
    auto end_mul = std::chrono::high_resolution_clock::now();
  
    auto duration_mul = std::chrono::duration_cast<std::chrono::microseconds>(end_mul - start_mul).count();

    if (print_matrices) {
        std::cout << "Matrix A:" << std::endl;
        print_matrix(A);
        std::cout << "\nMatrix B:" << std::endl;
        print_matrix(B);
        std::cout << "\nMatrix C (A * B):" << std::endl;
        print_matrix(C);
    }
  
    std::cout << "Matrix multiplication time: " << duration_mul << " microseconds" << std::endl;

    return 0;
}
