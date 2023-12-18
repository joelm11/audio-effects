#pragma once

void compute_hann_win(double *window_buffer, int win_size, int an_hop_size);

constexpr void compute_tri_win(double *window_buffer, int win_size);