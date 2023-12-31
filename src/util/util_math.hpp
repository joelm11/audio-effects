#pragma once
#include <numbers>
#include <cmath>

namespace math {
    void compute_hann_win(double *window_buffer, int win_size, int an_hop_size);

    void compute_tri_win(double *window_buffer, int win_size);

    template <typename T>
    constexpr T sinc(T x) {
        return x == 0 ? 1 : std::sin(std::numbers::pi * x) / (std::numbers::pi * x);
    }
}