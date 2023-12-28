#pragma once
#include <complex.h>
#include <cstddef>
#include <string>
#include <iostream>
#include <fftw3.h>
#include <sndfile.h>
#include "status_codes.hpp"
#include "vocoder_types.hpp"

class vocoder {
    public:
        using status = util::status_codes;
        using dtype  = double;
        using string = std::string;
        using pair   = std::pair<int, int>;
        using complex = std::complex<dtype>;

        voc_args user_args;

        vocoder () = delete;

        vocoder (const voc_args &init_args);

        vocoder (const voc_args &init_args, int frame_size);

        ~vocoder (); 

        /* Open input file for read, populate input buffer, store file format information */
        status vocoder_init();

        /* Phase vocoder analysis stage */
        /* Apply window function to present buffer frame and compute mag and phase spectra */
        status analysis();

        /* Phase vocoder modification stage for robotization effect */
        status modify_phase_r();
        
        /* Phase vocoder modification stage for all-other effects */
        status modify_phase_t();

        /* Phase vocoder resynthesis phase */
        /* Take the IFFT of the modified frame, overlap add to output buff */
        status resynthesis();

        /* Read samples to input buffer */
        status read_samples(float *buffer, int buff_offset);

        /* Read samples to input buffer */
        status read_samples(double *buffer, int buff_offset, int num_samples);

    private:
        SF_INFO file_data;
        SNDFILE *input_fh;
        SNDFILE *output_fh;
        // Buffers
        const int frame_size = 1024;
        const int outbuff_size = 3 * frame_size;
        int outbuff_offset = 0;
        dtype *inbuff;
        dtype *outbuff;
        // Prior information needed for vocoder:
        // 1. Previous frame's phase
        // 2. Previous frame's modified phase
        dtype *prev_phase;
        dtype *prev_synth_phase;
        // Buffers needed for FFTW
        complex *fftw_input;
        complex *fftw_output;
        fftw_plan fft, ifft;
        // Windows?
        dtype *window_hann;
        dtype *window_tri;
        // Misc.
        const int analysis_hop_size = frame_size / 4;
        int synthesis_hop_size = analysis_hop_size;
};