#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>

// 行列の生成
std::vector<std::vector<double>> generate_matrix(int rows, int cols);

// 行列積の計算
std::vector<std::vector<double>> multiply_matrices(
    const std::vector<std::vector<double>>& A, 
    const std::vector<std::vector<double>>& B
);

#endif // MATRIX_HPP
