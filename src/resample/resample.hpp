#pragma once
#include "util_math.hpp"

template <typename T, int n_zc, int n_per_zc>
class resampler {
    public: 
        using dtype = T;
        
        const int FILTER_LEN = n_zc * n_per_zc + 1;
        const dtype LPF_ROLLOFF = 0.945;
        
        constexpr resampler () : interp_lpf(), dinterp_lpf() {
            for (int i = 0; i < FILTER_LEN; ++i) {
                interp_lpf[i] = LPF_ROLLOFF * math::sinc((LPF_ROLLOFF * i) / FILTER_LEN);
            }
            for (int i = 0; i < FILTER_LEN - 1; ++i) {
                dinterp_lpf[i] = interp_lpf[i + 1] - dinterp_lpf[i];
            }
        }

        void resample ();

    private:
        dtype  interp_lpf[n_zc * n_per_zc + 1];
        dtype dinterp_lpf[n_zc * n_per_zc + 1];
};