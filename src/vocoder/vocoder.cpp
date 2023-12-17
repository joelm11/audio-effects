#include <cstddef>
#include "sndfile.h"
#include "vocoder.hpp"
#include "vocoder_types.hpp"

vocoder::vocoder (const voc_args &init_args) : user_args(init_args) { }

vocoder::vocoder (const voc_args &init_args, int frame_size) : user_args(init_args), frame_size(frame_size) { }

vocoder::~vocoder() {
    delete [] inbuff;
    delete [] outbuff;
}

vocoder::status vocoder::vocoder_init() {
    input_fh = sf_open(user_args.input_filename.c_str(), SFM_READ, &file_data);
    if (input_fh == NULL) {
        return status::FILE_READ_FAIL;
    }
    inbuff  = new dtype[frame_size * 2];
    outbuff = new dtype[frame_size * 2];
    status ret_status = read_samples(inbuff, PAST);
    ret_status = read_samples(inbuff, PRESENT);
    return ret_status;
}

vocoder::status vocoder::read_samples(float *buffer, int buff_offset) {
    sf_count_t samples_read = sf_read_float(input_fh, buffer + buff_offset, frame_size);
    if (samples_read < frame_size) {
        return status::BUFFER_NOT_FULL;
    }
    return status::SUCCESS;
}

vocoder::status vocoder::read_samples(double *buffer, int buff_offset) {
    sf_count_t samples_read = sf_read_double(input_fh, buffer + buff_offset, frame_size);
    if (samples_read < frame_size) {
        return status::BUFFER_NOT_FULL;
    }
    return status::SUCCESS;
}