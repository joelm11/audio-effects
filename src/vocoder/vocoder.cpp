#include <cstddef>
#include "vocoder.hpp"
#include "vocoder_types.hpp"

vocoder::vocoder (const voc_args &init_args) : user_args(init_args) { }

vocoder::vocoder (const voc_args &init_args, int frame_size) : user_args(init_args), frame_size(frame_size) { }

vocoder::status vocoder::vocoder_init() {
    return vocoder::status::SUCCESS;
}