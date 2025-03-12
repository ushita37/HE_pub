#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <seal/seal.h>

using Matrix = std::vector<std::vector<double>>;
using namespace std;
using namespace seal;

// 行列の生成
std::vector<std::vector<double>> generate_matrix(int rows, int cols, int external_seed);

// 畳み込み
vector<vector<Ciphertext>> encrypted_matrix_convolution(
    vector<vector<Ciphertext>> enc_input, 
    vector<vector<Ciphertext>> enc_kernel, 
    vector<vector<Ciphertext>> enc_output, 
    Evaluator &evaluator, 
    RelinKeys relin_keys, 
    double scale
);

#endif // MATRIX_HPP
