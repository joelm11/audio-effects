#pragma once
#include "fftw3.h"
#include <complex>
#include <cstring>
#include <iostream>

template <typename T>
class pitch {
    public:
        using dtype = T;
        using complex = std::complex<dtype>;
        pitch (int frame_size) : frame_size(frame_size), nfft(2 * frame_size) {
            fftw_input = (complex*) fftw_malloc(sizeof(fftw_complex) * nfft);
            fftw_output = (complex*) fftw_malloc(sizeof(fftw_complex) * nfft);
            fft = fftw_plan_dft_1d(nfft, reinterpret_cast<fftw_complex*>(fftw_input),
                                reinterpret_cast<fftw_complex*>(fftw_output), FFTW_FORWARD, FFTW_MEASURE);
            ifft = fftw_plan_dft_1d(nfft, reinterpret_cast<fftw_complex*>(fftw_output),
                                reinterpret_cast<fftw_complex*>(fftw_input), FFTW_BACKWARD, FFTW_MEASURE);
            corrs        = new dtype[frame_size]();
            corrs_lagged = new dtype[frame_size]();
            diff         = new dtype[frame_size]();
        } 
    
        ~pitch () {
            delete [] corrs;
            delete [] corrs_lagged;
            delete [] diff;
            fftw_destroy_plan(fft);
            fftw_destroy_plan(ifft);
            fftw_free(fftw_input);
            fftw_free(fftw_output);
        }

        int find_fund_freq(const auto &sample_buff, int fs, int max_freq = 3500, double threshold = 1.0) {
            difference(sample_buff);
            normalize_diff();
            // Find local minima below some threshold
            int max_freq_ind = fs / max_freq + 1;
            for (int lag = max_freq_ind; lag < frame_size; ++lag) {
                if (diff[lag] < threshold) {
                    while (diff[lag] > diff[lag + 1]) {
                        ++lag;
                    }
                    return (fs / (lag + 1));
                }
            }
            return -1;
        }

    private:
        void difference(const auto& samples) {
            // Cumulative sum of squares
            dtype sum = 0;
            for (int i = 0; i < frame_size; ++i) {
                corrs[i] = samples[i] * samples[i] + sum;
                sum = corrs[i];
                // Copy sample to FFT buffer for autocorrellation
                fftw_input[i].real(samples[i]);
            }
            // Efficient autocorrellation
            fftw_execute(fft);
            for (int i = 0; i < nfft; ++i) {
                fftw_output[i] = fftw_output[i] * std::conj(fftw_output[i]);
            }
            fftw_execute(ifft);
            for (int i = 0; i < frame_size; ++i) {
                corrs_lagged[i] = fftw_input[i].real() / nfft;
            }
            // Difference function
            // This uses an autocorrellation trick from the cumulative sum
            diff[0] = 2 * corrs[frame_size - 1] - 2 * corrs_lagged[0];
            for (int i = 1; i < frame_size; ++i) {
                diff[i] = 2 * corrs[frame_size - 1] - corrs[i - 1] - 2 * corrs_lagged[i];
            }
            // Re-zero FFT input buffer after use
            for (int i = 0; i < nfft; ++i) {
                fftw_input[i]  = 0;
            }
        }

        void normalize_diff() {
            diff[0] = 1;
            dtype sum = diff[1];
            for (int i = 1; i < frame_size; ++i) {
                diff[i] *= i / sum;
                sum += diff[i + 1];
            }            
        }

        // Buffers
        complex *fftw_input;
        complex *fftw_output;
        fftw_plan fft, ifft;
        dtype *corrs;
        dtype *corrs_lagged;
        dtype *diff;
               
        int frame_size = 1024;
        int nfft = 0;
};