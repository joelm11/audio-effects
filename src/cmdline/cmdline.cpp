#include "cmdline.hpp"
#include "status_codes.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <string_view>

util::status_codes util::parse_args(const int argc, char *argv[]) {
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
        inputf = cmdl_args[0];
    }
    // Set input and output file name. Select chosen effect and modification factor.
    for (int i = 0; i < argc; ++i) {
        if (std::strcmp(argv[i], "-e") == 0) {
            effect = argv[i + 1];
        }
        if (std::strcmp(argv[i], "-o") == 0) {
            outpf = argv[i + 1];
        }
        if (std::strcmp(argv[i], "-mf") == 0) {
            mod_factor = argv[i + 1];
        }
    }
    if (outpf == "") {
        outpf = "out_" + inputf;
    }

    printf("Input filename: %s, Output filename: %s, Effect: %s, Mod Factor: %s\n",
            inputf.c_str(), outpf.c_str(), effect.c_str(), mod_factor.c_str());
    return status_codes::SUCCESS;
}

int util::effect_as_int(const std::string &effect) {
    if (effect == "robot") {
        return 0;
    }
    else if (effect == "tstretch") {
        return 1;
    }
    else if (effect == "pitch") {
        return 2;
    }
    else if (effect == "autotune") {
        return 3;
    }
    else {
        return -1;
    }
}