#include "matrix.hpp"
#include <vector>
#include <random>
#include <omp.h>


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

// 畳み込み計算（OpenMPを用いて並列化）
Matrix convolveDirect(const Matrix &input, const Matrix &kernel)
{
    int inputRows = input.size();
    int inputCols = input[0].size();
    int kernelRows = kernel.size();
    int kernelCols = kernel[0].size();

    // 出力行列のサイズを計算
    int outputRows = inputRows - kernelRows + 1;
    int outputCols = inputCols - kernelCols + 1;

    Matrix output(outputRows, std::vector<double>(outputCols, static_cast<double>(0)));

    // 畳み込み演算を並列化
    #pragma omp parallel for collapse(2) schedule(static) // 二重ループの並列化
    for (int i = 0; i < outputRows; ++i)
    {
        for (int j = 0; j < outputCols; ++j)
        {
            double sum = 0.0;
            for (int ki = 0; ki < kernelRows; ++ki)
            {
                for (int kj = 0; kj < kernelCols; ++kj)
                {
                    sum += input[i + ki][j + kj] * kernel[ki][kj];
                }
            }
            
            output[i][j] = sum;
        }
    }

    return output;
}
