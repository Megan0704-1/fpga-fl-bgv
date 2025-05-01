#include <chrono>
#include <iostream>
#include <vector>
#include "constants.hpp"
#include "polynomial_multiplication.hpp"
#include "test_utils.h"

static void fill_inputs(std::vector<std::pair<std::vector<data_t>, std::vector<data_t>>>& inputs) {
    inputs.reserve(TEST_RUNS);
    for (int t = 0; t < TEST_RUNS; ++t) {
        std::vector<data_t> A(POLYNOMIAL_DEGREE), B(POLYNOMIAL_DEGREE);
        for (int i = 0; i < POLYNOMIAL_DEGREE; ++i) {
            A[i] = (i + t) % CIPHERTEXT_MODULUS;
            B[i] = (2*i + t) % CIPHERTEXT_MODULUS;
        }
        inputs.emplace_back(std::move(A), std::move(B));
    }
}

int main() {
    using clock = std::chrono::high_resolution_clock;
    std::vector<std::pair<std::vector<data_t>, std::vector<data_t>>> inputs;
    fill_inputs(inputs);
    std::vector<data_t> output(POLYNOMIAL_DEGREE);

    // Warm up
    for (int i = 0; i < 5; ++i) {
        polynomial_multiplication(inputs[i].first.data(),
                                  inputs[i].second.data(),
                                  output.data());
    }

    // Time the runs
    auto t0 = clock::now();
    for (auto &p : inputs) {
        polynomial_multiplication(p.first.data(),
                                  p.second.data(),
                                  output.data());
    }
    auto t1 = clock::now();

    double total_us = std::chrono::duration<double, std::micro>(t1 - t0).count();
    double avg_us   = total_us / inputs.size();

    std::cout
        << "Ran " << inputs.size() << " polynomial multiplications in "
        << total_us << " µs\n"
        << "Average: " << avg_us << " µs per call\n";
    return 0;
}
