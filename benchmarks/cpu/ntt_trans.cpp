#include <chrono>
#include <iostream>
#include <vector>
#include "constants.hpp"
#include "ntt_transform.hpp"
#include "modulo_reduction.hpp"
#include "test_utils.h"

static void fill_inputs(std::vector<std::vector<data_t>>& inputs) {
    inputs.reserve(TEST_RUNS);
    for (int t = 0; t < TEST_RUNS; ++t) {
        std::vector<data_t> A(POLYNOMIAL_DEGREE);
        for (int i = 0; i < POLYNOMIAL_DEGREE; ++i) {
            A[i] = (i + t) % CIPHERTEXT_MODULUS;
        }
        inputs.push_back(std::move(A));
    }
}

int main() {
    using clock = std::chrono::high_resolution_clock;
    std::vector<std::vector<data_t>> inputs;
    fill_inputs(inputs);

    std::vector<data_t> out_forward(POLYNOMIAL_DEGREE);
    std::vector<data_t> out_inverse(POLYNOMIAL_DEGREE);

    // Warm up
    for (int i = 0; i < 5; ++i) {
        ntt_transform(inputs[i].data(), out_forward.data());
        intt_transform(out_forward.data(), out_inverse.data());
    }

    // Time only the forward+inverse combined
    auto t0 = clock::now();
    for (auto &A : inputs) {
        ntt_transform(A.data(), out_forward.data());
        intt_transform(out_forward.data(), out_inverse.data());
    }
    auto t1 = clock::now();

    double total_us = std::chrono::duration<double, std::micro>(t1 - t0).count();
    double avg_us   = total_us / inputs.size();

    std::cout
        << "Ran " << inputs.size() << " NTT+INTT transforms in "
        << total_us << " µs\n"
        << "Average: " << avg_us << " µs per combined call\n";
    return 0;
}
