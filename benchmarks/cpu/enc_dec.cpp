#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include "constants.hpp"
#include "polynomial_multiplication.hpp"
#include "encryption.hpp"
#include "test_utils.h"

using input_type = std::vector<std::vector<data_t>>;

static void fill_inputs(input_type &inputs) {
    inputs.reserve(TEST_RUNS);
    std::mt19937 gen(0);
    std::uniform_int_distribution<int> dist(-PLAINTEXT_MODULUS/2, PLAINTEXT_MODULUS/2);
    for(int i=0; i<TEST_RUNS; ++i) {
        std::vector<data_t> pt(POLYNOMIAL_DEGREE);
        for(int j=0; j<POLYNOMIAL_DEGREE; ++j) {
            pt[j] = dist(gen);
        }
        inputs.push_back(pt);
    }
}

void keygen(std::mt19937 &rng, data_t *sk, data_t *e1, data_t *e2, data_t *pk1, data_t *pk2, data_t *r) {
    std::uniform_int_distribution<int> dist_noise(-1, 1);
    std::uniform_int_distribution<int> dist_mod(0, CIPHERTEXT_MODULUS-1);

    static data_t a_prime[POLYNOMIAL_DEGREE];
    data_t temp[POLYNOMIAL_DEGREE];
    data_t error[POLYNOMIAL_DEGREE];

    // private keygen
    for(int i=0; i<POLYNOMIAL_DEGREE; ++i) {
        sk[i] = dist_noise(rng);
        e1[i] = dist_noise(rng);
        e2[i] = dist_noise(rng);
        error[i] = dist_noise(rng);
        a_prime[i] = dist_mod(rng);
        r[i] = dist_noise(rng);
    }

    polynomial_multiplication(a_prime, sk, temp);

    // public keygen
    for(int i=0; i<POLYNOMIAL_DEGREE; ++i) {
        pk1[i] = (temp[i] + PLAINTEXT_MODULUS*error[i] + CIPHERTEXT_MODULUS) % CIPHERTEXT_MODULUS;
        pk2[i] = (CIPHERTEXT_MODULUS - a_prime[i]) % CIPHERTEXT_MODULUS;
    }
}

int main() {
    using clock = std::chrono::high_resolution_clock;

    std::mt19937 rng(12345);
    std::vector<data_t> sk(POLYNOMIAL_DEGREE), pk1(POLYNOMIAL_DEGREE), pk2(POLYNOMIAL_DEGREE), e1(POLYNOMIAL_DEGREE), e2(POLYNOMIAL_DEGREE), r(POLYNOMIAL_DEGREE);
    keygen(rng, sk.data(), e1.data(), e2.data(), pk1.data(), pk2.data(), r.data());

    std::vector<data_t> plaintext(POLYNOMIAL_DEGREE), ct1(POLYNOMIAL_DEGREE), ct2(POLYNOMIAL_DEGREE);

    input_type inputs;
    fill_inputs(inputs);

    // warm up
    std::vector<data_t> output(POLYNOMIAL_DEGREE);
    for(int i=0; i<5; ++i) {
        encryption(e1.data(), e2.data(), r.data(), pk1.data(), pk2.data(), inputs[i].data(), ct1.data(), ct2.data());
        decryption(sk.data(), ct1.data(), ct2.data(), output.data());
    }

    // Time te run
    auto t0 = clock::now();
    for(auto &pt : inputs) {
        encryption(e1.data(), e2.data(), r.data(), pk1.data(), pk2.data(), pt.data(), ct1.data(), ct2.data());
        decryption(sk.data(), ct1.data(), ct2.data(), output.data());
    }
    auto t1 = clock::now();

    double total_us = std::chrono::duration<double, std::micro>(t1-t0).count();
    double avg_us = total_us / inputs.size();

    std::cout << "Ran " << inputs.size() << " encrypt+decrypt ops in "
              << total_us << " µs\n  Average: " << avg_us 
              << " µs per cycle\n";

    return 0;
}
