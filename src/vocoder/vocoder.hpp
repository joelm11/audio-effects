#pragma once
#include <cstddef>
#include <sndfile.h>
#include <string>
#include "sndfile.h"
#include <iostream>
#include "status_codes.hpp"
#include "vocoder_types.hpp"

class vocoder {
    public:
        using status = util::status_codes;
        using dtype  = float;
        using string = std::string;
        using pair   = std::pair<int, int>;

        vocoder () = delete;

        vocoder (const voc_args &init_args);

        vocoder (const voc_args &init_args, int frame_size);

        ~vocoder (); 

        /* Open input file for read, populate input buffer, store file format information */
        status vocoder_init();

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
};