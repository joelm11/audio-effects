#pragma once
#include <sndfile.h>
#include <algorithm>
#include "sndfile.h"
#include "status_codes.hpp"
#include "util_math.hpp"

/* Implementation of sinc interpolation presented by Julius Smith */
template <typename T, int n_zc, int n_per_zc>
class resampler {
    public: 
        using dtype = T;
        using status = util::status_codes;
        using string = std::string;
        
        const int FILTER_LEN = n_zc * n_per_zc + 1;
        const dtype LPF_ROLLOFF = 0.945;
        
        /* Calculate filter at construction time - to be used for arbitrary resampling rates */
        constexpr resampler () : interp_lpf(), dinterp_lpf() {
            for (int i = 0; i < FILTER_LEN; ++i) {
                interp_lpf[i] = LPF_ROLLOFF * math::sinc((LPF_ROLLOFF * i) / FILTER_LEN);
            }
            for (int i = 0; i < FILTER_LEN - 1; ++i) {
                dinterp_lpf[i] = interp_lpf[i + 1] - dinterp_lpf[i];
            }
        }

        ~resampler () {
            delete [] inbuff;
            delete [] outbuff;
            sf_close(input_fh);
            sf_close(output_fh);
        }

        status resampler_init (const string &infile, const string &outfile, const int samples_per_frame) {
            input_fh  = sf_open(infile, SFM_READ, &file_data);
            output_fh = sf_open(outfile, SFM_WRITE, &file_data);
            if (input_fh == NULL || output_fh == NULL) {
                return status::FILE_READ_FAIL;
            }
            frame_size = samples_per_frame;
            inbuff  = new dtype[2 * frame_size];
            if (sf_read_double(input_fh, inbuff, frame_size) == 0) {
                return status::FILE_READ_FAIL;
            }
            outbuff = new dtype[frame_size];
            return status::SUCCESS;
        }

        /* Resample file */
        void resample (int fs_orig, int fs_new) {
            dtype rs_ratio = static_cast<dtype>(fs_new) / fs_orig;
            int n, offset, outbuff_idx; 
            dtype frac, interp_factor, t = 0;
            bool loop_cond = true;
            while (resampler_fill_shift_buff() == status::BUFFER_FULL) {
                // Resample using range of samples available in buffer
                while (n < (frame_size + fs_2)) {
                    compute_vars(t, n, offset, interp_factor, true);
                    outbuff[outbuff_idx] = compute_filter_response(n, offset, interp_factor, outbuff_idx, true);
                    compute_vars(t, n, offset, interp_factor, false);
                    outbuff[outbuff_idx++] += compute_filter_response(n, offset, interp_factor, outbuff_idx, false);
                    if (outbuff_idx == frame_size) {
                        outbuff_idx %= frame_size;
                        reampler_write_shift_buff();
                    }
                    t += rs_ratio;
                }
            }
        }

        void compute_vars(dtype t, int &sample_index, int &initial_offset, dtype &interp_factor, bool lhs) {
            dtype frac;
            if (lhs) {
                frac = t - sample_index;
            }
            else {
                frac = 1.0 - (t - sample_index);
            }
            sample_index   = (int) t;
            initial_offset = frac * n_per_zc;
            interp_factor = (frac * n_per_zc) - initial_offset;
        }

        dtype compute_filter_response (int n, int init_offset, dtype interp_factor, int outbuff_idx, bool lhs) {
            double filter_response_sum = 0.0;
            int i_max;
            if (lhs) {
                i_max = std::min(n + 1, (lpf_size - init_offset) / n_per_zc);
            }
            else {
                i_max = std::min((2 * frame_size) - n - 1, (lpf_size - init_offset) / n_per_zc);
            }
            for (int i = 0; i < i_max; ++i) {
                filter_response_sum += interp_lpf[init_offset + i * n_per_zc];
                filter_response_sum += dinterp_lpf[init_offset + i * n_per_zc];
            }
            return filter_response_sum;
        }

        status resampler_fill_shift_buff () {
            std::copy(inbuff + fs_2, inbuff + frame_size + fs_2, inbuff);
            auto count = sf_read_double(input_fh, inbuff + frame_size, fs_2);
            return count < fs_2 ? status::BUFFER_NOT_FULL : status::BUFFER_FULL;
        }

        status reampler_write_shift_buff () {
            sf_write_double(output_fh, outbuff, frame_size);
            return status::SUCCESS;
        }

    private:
        int    lpf_size = n_zc * n_per_zc + 1;
        dtype  interp_lpf[n_zc * n_per_zc + 1];
        dtype  dinterp_lpf[n_zc * n_per_zc + 1];
        dtype  time_register;

        dtype *inbuff;
        dtype *outbuff;

        int frame_size = 1024;
        int fs_2 = frame_size / 2;

        SF_INFO file_data;
        SNDFILE *input_fh;
        SNDFILE *output_fh;
};