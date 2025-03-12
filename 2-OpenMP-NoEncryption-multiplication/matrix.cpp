#include "matrix.hpp"
#include <vector>
#include <random>
#include <omp.h>

// 行列の生成
std::vector<std::vector<double>> generate_matrix(int rows, int cols, int external_seed) {
    std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols));
    std::random_device rd;
    std::uniform_real_distribution<> dis(0, 10);

    #pragma omp for collapse(2) schedule(static) // 二重ループを並列
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::mt19937 gen(i*rows + j + external_seed*rows*cols);
            matrix[i][j] = dis(gen);
        }
    }

    return matrix;
}

// 行列積の計算（OpenMPを用いた並列化）
std::vector<std::vector<double> > multiply_matrices(
    const std::vector<std::vector<double> >& A, 
    const std::vector<std::vector<double> >& B
) {
    int rows = A.size();
    int cols = B[0].size();
    int common_dim = B.size();

    std::vector<std::vector<double> > C(rows, std::vector<double>(cols, 0));

    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            for (int k = 0; k < common_dim; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}
