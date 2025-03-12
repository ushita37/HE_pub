#include "seal/seal.h"
#include "matrix.hpp"

using namespace std;
using namespace seal;

vector<vector<Ciphertext>> encrypted_matrix_multiply(vector<vector<Ciphertext>> enc_A,  vector<vector<Ciphertext>> enc_B, vector<vector<Ciphertext>> enc_C, Evaluator &evaluator, RelinKeys relin_keys, double scale)
{
    int rows = enc_A.size();
    int cols = enc_B[0].size();
    int common_dim = enc_B.size();

#pragma omp parallel for collapse(2) schedule(static)   // parallelキーワードを追加
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            for (int k = 0; k < common_dim; ++k)
            {
                Ciphertext temp;
                evaluator.multiply(enc_A[i][k], enc_B[k][j], temp); // 掛け算
                evaluator.relinearize_inplace(temp, relin_keys);    // 線形処理
                evaluator.rescale_to_next_inplace(temp);            // リスケール

                // スケールを揃えて加算の準備をする
                temp.scale() = scale;
                enc_C[i][j].scale() = scale;
                
                evaluator.mod_switch_to_inplace(enc_C[i][j], temp.parms_id());

                evaluator.add_inplace(enc_C[i][j], temp);
            }
        }
    }

    return enc_C;
}

int main(int argc, char *argv[])
{
    int MATRIX_SIZE_A_ROWS, MATRIX_SIZE_A_COLS, MATRIX_SIZE_B_COLS;
    MATRIX_SIZE_A_ROWS = std::stoi(argv[1]); // 行列サイズ
    MATRIX_SIZE_A_COLS = std::stoi(argv[2]);
    MATRIX_SIZE_B_COLS = std::stoi(argv[3]);

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

    int external_seed_A = 0;
    int external_seed_B = 1;

    auto A = generate_matrix(MATRIX_SIZE_A_ROWS, MATRIX_SIZE_A_COLS, external_seed_A);
    auto B = generate_matrix(MATRIX_SIZE_A_COLS, MATRIX_SIZE_B_COLS, external_seed_B);

    int rows = A.size();
    int cols = B[0].size();
    int common_dim = B.size();

    vector<vector<double>> C(rows, vector<double>(cols, 0));     // 生データのままでの計算結果を格納
    vector<vector<double>> msg_C(rows, vector<double>(cols, 0)); // CKKSを用いた計算結果を格納

    // 生データの行列を平文空間に
    vector<vector<Plaintext>> plain_A(rows, vector<Plaintext>(common_dim));
    vector<vector<Plaintext>> plain_B(common_dim, vector<Plaintext>(cols));
    vector<vector<Plaintext>> plain_before_C(rows, vector<Plaintext>(cols));  // 初期化

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < common_dim; ++j)
        {
            encoder.encode(A[i][j], scale, plain_A[i][j]);
        }
    }

    for (int i = 0; i < common_dim; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            encoder.encode(B[i][j], scale, plain_B[i][j]);
        }
    }

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < common_dim; ++j)
        {
            encoder.encode(msg_C[i][j], scale, plain_before_C[i][j]);
        }
    }

    // 平文を暗号文に
    vector<vector<Ciphertext>> enc_A(rows, vector<Ciphertext>(common_dim));
    vector<vector<Ciphertext>> enc_B(common_dim, vector<Ciphertext>(cols));
    vector<vector<Ciphertext>> enc_C(rows, vector<Ciphertext>(cols));  // 初期化

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < common_dim; ++j)
        {
            encryptor.encrypt(plain_A[i][j], enc_A[i][j]); // スケールは指定しなくて良い
        }
    }

    for (int i = 0; i < common_dim; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            encryptor.encrypt(plain_B[i][j], enc_B[i][j]); // スケールは指定しなくて良い
        }
    }

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < common_dim; ++j)
        {
            encryptor.encrypt(plain_before_C[i][j], enc_C[i][j]);
        }
    }

    // 暗号化された行列の行列積を計算
    auto start_mul = chrono::high_resolution_clock::now();
    enc_C = encrypted_matrix_multiply(enc_A, enc_B, enc_C, evaluator, relin_keys, scale);
    auto end_mul = chrono::high_resolution_clock::now();

    // 計算結果を平文にして生データまで戻す
    vector<vector<Plaintext>> plain_C(rows, vector<Plaintext>(cols));
    vector<vector<double>> calcC(rows, vector<double>(cols, 0));

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            decryptor.decrypt(enc_C[i][j], plain_C[i][j]); // スケールは指定しなくて良い
        }
    }

    vector<double> result;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            encoder.decode(plain_C[i][j], result);
            calcC[i][j] = result[0];
        }
    }

    // 計算時間を出力
    auto duration_mul = chrono::duration_cast<chrono::microseconds>(end_mul - start_mul).count();
    cout << "\nMatrix multiplication time: " << duration_mul << " microseconds" << endl;

    return 0;
}
