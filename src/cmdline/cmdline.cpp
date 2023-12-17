#include "cmdline.hpp"
#include "status_codes.hpp"
#include "vocoder_types.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <string_view>

util::status_codes util::parse_args(const int argc, char *argv[], voc_args &vargs) {
    std::string inputf, outpf, effect, mod_factor;
    if (argc == 1) {
        std::cout << "Please specify an input .wav file as first argument.\n";
        return status_codes::ERROR;
    }
    const std::vector<std::string_view> cmdl_args(argv + 1, argv + argc);
    if (cmdl_args[0].find(".wav") == std::string::npos) {
        std::cout << "Please specify an input .wav file as first argument.\n";
        return status_codes::ERROR;
    }
    else {
        vargs.input_filename = cmdl_args[0];
    }
    // Set input and output file name. Select chosen effect and modification factor.
    for (int i = 0; i < argc; ++i) {
        if (std::strcmp(argv[i], "-e") == 0) {
            vargs.sel_effect = effect_as_enum(argv[i + 1]);
        }
        if (std::strcmp(argv[i], "-o") == 0) {
            vargs.output_filename = argv[i + 1];
        }
        if (std::strcmp(argv[i], "-mf") == 0) {
            // mod_factor = argv[i + 1];
            vargs.mod_factor = std::pair<int, int> (1, 1);
        }
    }
    if (vargs.output_filename == "") {
        auto pos = vargs.input_filename.find(".");
        vargs.output_filename = vargs.input_filename;
        vargs.output_filename = vargs.output_filename.insert(pos, "_out");
    }

    printf("Input filename: %s\nOutput filename: %s\nEffect: %d\nMod Factor: %d/%d\n",
            vargs.input_filename.c_str(), vargs.output_filename.c_str(), vargs.sel_effect, 
            vargs.mod_factor.first, vargs.mod_factor.second);
    return status_codes::SUCCESS;
}

voc_effect util::effect_as_enum(const std::string &effect) {
    if (effect == "robot") {
        return voc_effect::ROBOT;
    }
    else if (effect == "tstretch") {
        return voc_effect::TIME_STRETCH;
    }
    else if (effect == "pitch") {
        return voc_effect::PITCH_SHIFT;
    }
    else if (effect == "autotune") {
        return voc_effect::AUTO_TUNE;
    }
    else {
        return voc_effect::NOT_IMPLEMENTED;
    }
}