#include <chrono>
#include <iostream>
#include <vector>
#include "constants.hpp"
#include "modulo_reduction.hpp"
#include "test_utils.h"

static void fill_inputs(std::vector<std::pair<data_t,data_t>>& v) {
    const data_t P = 256;
    v.reserve(TEST_RUNS);
    for (int i = 0; i < TEST_RUNS; ++i) {
        data_t a = (i * 12345 + 6789) % 512 - 128;  // range [-128, 383]
        data_t b = P;
        v.emplace_back(a, b);
    }
}

int main() {
    using clock = std::chrono::high_resolution_clock;
    std::vector<std::pair<data_t,data_t>> inputs;
    fill_inputs(inputs);

    // warm up cache / branch predict
    for (int i = 0; i < 1000; ++i) {
        auto [a, b] = inputs[i];
        volatile data_t t = modulo_reduction_neg(a, b);
        (void)t;
    }

    // timed loop
    auto t0 = clock::now();
    for (auto &p : inputs) {
        data_t out = modulo_reduction_neg(p.first, p.second);
        (void)out;
    }
    auto t1 = clock::now();

    double total_us = std::chrono::duration<double,std::micro>(t1 - t0).count();
    double avg_us   = total_us / inputs.size();

    std::cout
      << "Ran " << inputs.size()
      << " calls in " << total_us << " µs\n"
      << "Average: " << avg_us << " µs per modulo_reduction_neg()\n";
    return 0;
}
