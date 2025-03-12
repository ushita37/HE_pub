#include "matrix.hpp"
#include <random>

using namespace std;
using namespace seal;


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

// 畳み込み演算
vector<vector<Ciphertext>> encrypted_matrix_convolution(vector<vector<Ciphertext>> enc_input, vector<vector<Ciphertext>> enc_kernel, vector<vector<Ciphertext>> enc_output, Evaluator &evaluator, RelinKeys relin_keys, double scale)
{
    int inputRows = enc_input.size();
    int inputCols = enc_input[0].size();
    int kernelRows = enc_kernel.size();
    int kernelCols = enc_kernel[0].size();

    // 出力行列のサイズを計算
    int outputRows = inputRows - kernelRows + 1;
    int outputCols = inputCols - kernelCols + 1;

    // OpenMPを使用して並列化
    #pragma omp parallel for collapse(2) schedule(static) // 二重ループの並列化
    for (int i = 0; i < outputRows; ++i)
    {
        for (int j = 0; j < outputCols; ++j)
        {
            Ciphertext sum;
            bool initialized = false;
            for (int ki = 0; ki < kernelRows; ++ki)
            {
                for (int kj = 0; kj < kernelCols; ++kj)
                {
                    Ciphertext temp;

                    evaluator.multiply(enc_input[i + ki][j + kj], enc_kernel[ki][kj], temp); // 掛け算
                    evaluator.relinearize_inplace(temp, relin_keys);                         // 線形化
                    evaluator.rescale_to_next_inplace(temp);                                 // リスケール

                    // スケールを合わせる
                    temp.scale() = scale;

                    if (initialized)
                    {
                        evaluator.mod_switch_to_inplace(sum, temp.parms_id());
                        evaluator.add_inplace(sum, temp);
                    }
                    else
                    {
                        sum = temp;
                        initialized = true;
                    }
                }
            }
            #pragma omp critical
            {
                enc_output[i][j] = sum;
            }
        }
    }

    return enc_output;
}
