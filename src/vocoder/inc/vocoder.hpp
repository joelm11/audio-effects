#pragma once
#include <complex.h>
#include <complex>
#include <cstddef>
#include <string>
#include <iostream>
#include <vector>
#include <fftw3.h>
#include <sndfile.h>
#include "status_codes.hpp"
#include "vocoder_types.hpp"
#include "pitch.hpp"

class vocoder {
    public:
        using status = util::status_codes;
        using dtype  = double;
        using string = std::string;
        using pair   = std::pair<int, int>;
        using complex = std::complex<dtype>;

        vocoder () = delete;

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
    
    private:
        /* Additional analysis required for autotune feature */
        void autotune_analysis();

        /* Read samples to input buffer */
        status read_samples(float *buffer, int buff_offset);

        /* Read samples to input buffer */
        status read_samples(double *buffer, int buff_offset, int num_samples);

    public:
        voc_args user_args;
        int frame_size;
        SF_INFO file_data;
        std::vector<dtype> alphas;

    private:
        SNDFILE *input_fh;
        SNDFILE *output_fh;
        // Buffers
        dtype *inbuff;
        dtype *outbuff;
        dtype *prev_phase;
        dtype *prev_synth_phase;
        dtype *window_hann;
        // Buffers needed for FFTW
        complex *fftw_input;
        complex *fftw_output;
        fftw_plan fft, ifft;
        // Misc.
        const int analysis_hop_size = frame_size / 4;
        int synthesis_hop_size = analysis_hop_size;
        const int outbuff_size = 3 * frame_size;
        int outbuff_offset = 0;
        // Autotune
        pitch<dtype> *pitchfind;
        const int mva_size = 32;
        dtype prev_alphas[32] {1};
        dtype mva;
        int mva_idx = 0;
};