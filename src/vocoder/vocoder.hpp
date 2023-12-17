#pragma once

#include <cstddef>
#include <sndfile.h>
#include <string>
#include "sndfile.h"
#include <iostream>
#include "status_codes.hpp"
#include "vocoder_types.hpp"


template <typename T = float, size_t frame_size = 1024>
class vocoder {
    public:
        using status = util::status_codes;
        using dtype  = T;
        using string = std::string;
        using pair   = std::pair<int, int>;
        
        // enum effect {
        //     ROBOT = 0,
        //     TIME_STRETCH,
        //     PITCH_SHIFT,
        //     AUTO_TUNE
        // };

        // struct voc_args{
        //     std::string input_filename;
        //     std::string output_filename;
        //     effect sel_effect;
        //     pair mod_factor;
        // };

        vocoder () = delete;

        // vocoder<T, frame_size> (effect sel_effect, std::string input_fname, std::string outp_fname);

        vocoder<T, frame_size> (voc_args init_args);
 

        /* Open input file for read, populate input buffer, store file format information */
        // status vocoder_init(const std::string &input_filename) {
        //     status ret_status = status::SUCCESS;
        //     input_fh = sf_open(input_filename.c_str(), SFM_READ, &file_data);
        //     if (input_fh == NULL) {
        //         ret_status = status::FILE_READ_FAIL;
        //     }
        //     ret_status = read_samples(input_buff + PAST);
        //     ret_status = read_samples(input_buff + PRESENT);
        //     return ret_status == status::FILE_READ_SUCCESS ? status::SUCCESS : status::ERROR;
        // }

        // vocoder_init<T, frame_size> (voc_args args);

        // Default read function if datatype not supported.
        template<typename dummy_type = T>
        status read_samples(dtype *buffer) {
            std::cout << "Attempting to process samples with an unsupported datatype\n";
            return status::FILE_READ_FAIL;
        }

        // Read float samples to input buffer.
        template<> status read_samples<float>(dtype *buffer) {
            sf_count_t samples_read = sf_read_float(input_fh, input_buff, frame_size);
            if (samples_read != frame_size) {
                return status::FILE_READ_FAIL;
            }
            return status::FILE_READ_SUCCESS;
        }

        // Read double samples to input buffer.
        template<> status read_samples<double>(dtype *buffer) {
            sf_count_t samples_read = sf_read_double(input_fh, input_buff, frame_size);
            if (samples_read != frame_size) {
                return status::FILE_READ_FAIL;
            }
            return status::FILE_READ_SUCCESS;
        }


    private:
        int load_inbuff();
        int write_out();

        // Private Members //
        SF_INFO file_data;
        SNDFILE *input_fh;
        SNDFILE *output_fh;
        string input_fn;
        string outp_fn;
        effect user_effect;
        pair user_mod_factor;
        // Buffers
        const int PAST = 0;
        const int PRESENT = frame_size;
        dtype input_buff[frame_size * 2];
        dtype output_buff[frame_size * 2];
};