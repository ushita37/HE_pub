#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>

// 行列型の定義
using Matrix = std::vector<std::vector<double>>;

// 行列の生成
std::vector<std::vector<double>> generate_matrix(int rows, int cols, int external_seed);

// 畳み込み
Matrix convolveDirect(const Matrix &input, const Matrix &kernel);

#endif // MATRIX_HPP
