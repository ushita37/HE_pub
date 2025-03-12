#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

#include "seal/seal.h"
#include "matrix.hpp"

using namespace std;
using namespace seal;
using Matrix = std::vector<std::vector<double>>;

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
            enc_output[i][j] = sum;
        }
    }

    return enc_output;
}

// 行列を出力する関数
void print_matrix(const vector<vector<double>> &matrix, const string &name)
{
    cout << name << ":" << endl;
    for (const auto &row : matrix)
    {
        for (double val : row)
        {
            cout << setw(10) << fixed << setprecision(4) << val << " ";
        }
    }
}

int main(int argc, char *argv[])
{
    int INPUT_SIZE_ROWS, INPUT_SIZE_COLS, KERNEL_SIZE_ROWS, KERNEL_SIZE_COLS;
    INPUT_SIZE_ROWS = INPUT_SIZE_COLS = std::stoi(argv[1]); // 入力の行数、列数
    KERNEL_SIZE_ROWS = KERNEL_SIZE_COLS = std::stoi(argv[2]);   // カーネルの行数、列数

    // SEALコンテキストの初期化
    EncryptionParameters parms(scheme_type::ckks);
    size_t poly_modulus_degree = 8192;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::Create(poly_modulus_degree, {60, 40, 40, 60}));
    // CKKSで使用するスケール値
    double scale = pow(2.0, 40);

    SEALContext context(parms);

    // 鍵生成
    KeyGenerator keygen(context);
    auto secret_key = keygen.secret_key();
    PublicKey public_key;
    keygen.create_public_key(public_key);
    RelinKeys relin_keys;
    keygen.create_relin_keys(relin_keys);
    GaloisKeys gal_keys;
    keygen.create_galois_keys(gal_keys);
    Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, secret_key);

    CKKSEncoder encoder(context);

    // inputとkernelの行列生成
    auto input = generate_matrix(INPUT_SIZE_ROWS, INPUT_SIZE_COLS);
    auto kernel = generate_matrix(KERNEL_SIZE_ROWS, KERNEL_SIZE_COLS);
  
    int inputRows = input.size();
    int inputCols = input[0].size();
    int kernelRows = kernel.size();
    int kernelCols = kernel[0].size();

    // 出力行列のサイズを計算
    int outputRows = inputRows - kernelRows + 1;
    int outputCols = inputCols - kernelCols + 1;

    vector<vector<double>> output(outputRows, vector<double>(outputCols, 0)); // CKKSを用いた計算結果を格納

    // 生データの行列を平文空間に
    vector<vector<Plaintext>> plain_input(inputRows, vector<Plaintext>(inputCols));
    vector<vector<Plaintext>> plain_kernel(kernelRows, vector<Plaintext>(kernelCols));
    vector<vector<Plaintext>> plain_before_output(outputRows, vector<Plaintext>(outputCols)); // 初期化

    for (int i = 0; i < inputRows; ++i)
    {
        for (int j = 0; j < inputCols; ++j)
        {
            encoder.encode(input[i][j], scale, plain_input[i][j]);
        }
    }

    for (int i = 0; i < kernelRows; ++i)
    {
        for (int j = 0; j < kernelCols; ++j)
        {
            encoder.encode(kernel[i][j], scale, plain_kernel[i][j]);
        }
    }

    for (int i = 0; i < outputRows; ++i)
    {
        for (int j = 0; j < outputCols; ++j)
        {
            encoder.encode(output[i][j], scale, plain_before_output[i][j]);
        }
    }

    // 平文を暗号文に
    vector<vector<Ciphertext>> enc_input(inputRows, vector<Ciphertext>(inputCols));
    vector<vector<Ciphertext>> enc_kernel(kernelRows, vector<Ciphertext>(kernelCols));
    vector<vector<Ciphertext>> enc_output(outputRows, vector<Ciphertext>(outputCols)); // 初期化

    for (int i = 0; i < inputRows; ++i)
    {
        for (int j = 0; j < inputCols; ++j)
        {
            encryptor.encrypt(plain_input[i][j], enc_input[i][j]); // スケールは指定しなくて良い
        }
    }

    for (int i = 0; i < kernelRows; ++i)
    {
        for (int j = 0; j < kernelCols; ++j)
        {
            encryptor.encrypt(plain_kernel[i][j], enc_kernel[i][j]); // スケールは指定しなくて良い
        }
    }

    for (int i = 0; i < outputRows; ++i)
    {
        for (int j = 0; j < outputCols; ++j)
        {
            encryptor.encrypt(plain_before_output[i][j], enc_output[i][j]);
        }
    }

    // 暗号化された行列の畳み込みを計算
    auto start_mul = chrono::high_resolution_clock::now();
    enc_output = encrypted_matrix_convolution(enc_input, enc_kernel, enc_output, evaluator, relin_keys, scale);
    auto end_mul = chrono::high_resolution_clock::now();

    // 計算結果を平文にして生データまで戻す
    vector<vector<Plaintext>> plain_output(outputRows, vector<Plaintext>(outputCols));
    vector<vector<double>> calc_output(outputRows, vector<double>(outputCols, 0));

    for (int i = 0; i < outputRows; ++i)
    {
        for (int j = 0; j < outputCols; ++j)
        {
            decryptor.decrypt(enc_output[i][j], plain_output[i][j]); // スケールは指定しなくて良い
        }
    }

    vector<double> result;
    for (int i = 0; i < outputRows; ++i)
    {
        for (int j = 0; j < outputCols; ++j)
        {
            encoder.decode(plain_output[i][j], result);
            calc_output[i][j] = result[0];
        }
    }

    // 計算時間を出力
    auto duration_mul = chrono::duration_cast<chrono::microseconds>(end_mul - start_mul).count();
    cout << "\nMatrix convolution time: " << duration_mul << " microseconds" << endl;

    return 0;
}
