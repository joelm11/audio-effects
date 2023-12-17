#include "vocoder.hpp"
#include <cstddef>

template <typename T, size_t frame_size>
vocoder<T, frame_size>::vocoder (voc_args init_args) {
    input_fn        = init_args.input_filename;
    outp_fn         = init_args.output_filename;
    user_effect     = init_args.sel_effect;
    user_mod_factor = init_args.mod_factor;
}

