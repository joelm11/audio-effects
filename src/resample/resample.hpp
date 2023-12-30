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
            inbuff  = new dtype[frame_size * 2];
            outbuff = new dtype[frame_size];
            return status::SUCCESS;
        }

        /* Resample file */
        void resample (int fs_orig, int fs_new) {
            
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
        dtype  interp_lpf[n_zc * n_per_zc + 1];
        dtype dinterp_lpf[n_zc * n_per_zc + 1];

        dtype *inbuff;
        dtype *outbuff;

        int frame_size = 1024;
        int fs_2 = frame_size / 2;

        SF_INFO file_data;
        SNDFILE *input_fh;
        SNDFILE *output_fh;
};