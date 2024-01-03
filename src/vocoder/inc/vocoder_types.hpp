#pragma once
#include <string>

enum voc_effect {
    ROBOT = 0,
    TIME_STRETCH,
    PITCH_SHIFT,
    AUTO_TUNE,
    NOT_IMPLEMENTED
};

struct voc_args{
    std::string input_filename;
    std::string output_filename;
    voc_effect sel_effect;
    std::pair<int, int> mod_factor;
};