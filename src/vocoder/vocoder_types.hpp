#pragma once
#include <string>

enum effect {
    ROBOT = 0,
    TIME_STRETCH,
    PITCH_SHIFT,
    AUTO_TUNE
};

struct voc_args{
    std::string input_filename;
    std::string output_filename;
    effect sel_effect;
    std::pair<int, int> mod_factor;
};