#include "util_math.hpp"
#include <cmath>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

void compute_hann_win(double *window_buffer, int win_size, int an_hop_size) {
    const double a = 0.54, b = -0.46; 
    const double c_norm = std::sqrt((double)an_hop_size / win_size) / std::sqrt(4 * a * a + 2 * b * b);
    const double pi_win = M_PI / win_size;
    for (int i = 0; i < win_size; ++i) {
        window_buffer[i] = a + b * std::cos(2 * M_PI * i / win_size)  + pi_win;
        window_buffer[i] *= c_norm;
    }
}