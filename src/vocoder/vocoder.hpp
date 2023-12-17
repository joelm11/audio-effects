#pragma once
#include <complex.h>
#include <cstddef>
#include <sndfile.h>
#include <fftw3.h>
#include <string>
#include "fftw3.h"
#include "sndfile.h"
#include <iostream>
#include "status_codes.hpp"
#include "vocoder_types.hpp"

class vocoder {
    public:
        using status = util::status_codes;
        using dtype  = double;
        using string = std::string;
        using pair   = std::pair<int, int>;
        using complex = std::complex<dtype>;

        vocoder () = delete;

        vocoder (const voc_args &init_args);

        vocoder (const voc_args &init_args, int frame_size);

        ~vocoder (); 

        /* Open input file for read, populate input buffer, store file format information */
        status vocoder_init();

        /* Phase vocoder analysis stage */
        status analysis();

        /* Phase vocoder modification stage */
        status modify_phase();

        /* Phase vocoder resynthesis phase */
        status resynthesis();

        /* Read samples to input buffer */
        status read_samples(float *buffer, int buff_offset);

        /* Read samples to input buffer */
        status read_samples(double *buffer, int buff_offset);

    private:
        SF_INFO file_data;
        SNDFILE *input_fh;
        SNDFILE *output_fh;
        voc_args user_args;
        // Buffers
        int frame_size = 1024;
        const int PAST = 0;
        const int PRESENT = frame_size;
        dtype *inbuff;
        dtype *outbuff;
        // Prior information needed for vocoder:
        // 1. Previous frame's phase
        // 2. Previous frame's modified phase
        complex *prev_phase;
        complex *prev_synth_phase;
        // Buffers needed for FFTW
        complex *fftw_input;
        complex *fftw_output;
        fftw_plan p;
};