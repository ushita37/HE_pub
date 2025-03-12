#include "matrix.hpp"
#include <vector>
#include <random>

// 行列の生成
std::vector<std::vector<double>> generate_matrix(int rows, int cols) {
    std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 10);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = dis(gen);
        }
    }

    return matrix;
}

// 行列積の計算
std::vector<std::vector<double>> multiply_matrices(
    const std::vector<std::vector<double>>& A, 
    const std::vector<std::vector<double>>& B
) {
    int rows = A.size();
    int cols = B[0].size();
    int common_dim = B.size();

    std::vector<std::vector<double>> C(rows, std::vector<double>(cols, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            for (int k = 0; k < common_dim; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}
