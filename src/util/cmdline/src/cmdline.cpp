#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <string_view>
#include "cmdline.hpp"
#include "status_codes.hpp"
#include "vocoder_types.hpp"

util::status_codes util::parse_args(const int argc, char *argv[], voc_args &vargs) {
    if (argc == 1) {
        std::cout << "Please specify an input .wav file as first argument.\n";
        return status_codes::BAD_CMDL_ARGS;
    }
    const std::vector<std::string_view> cmdl_args(argv + 1, argv + argc);
    if (cmdl_args[0].find(".wav") == std::string::npos) {
        std::cout << "Please specify an input .wav file as first argument.\n";
        return status_codes::BAD_CMDL_ARGS;
    }
    else {
        vargs.input_filename = cmdl_args[0];
    }
    // Set input and output file name. Select chosen effect and modification factor.
    for (auto itr = cmdl_args.begin(); itr != cmdl_args.end(); ++itr) {
        if (*itr == "-e") {
            if (itr + 1 != cmdl_args.end()) {
                vargs.sel_effect = effect_as_enum(*(++itr));
            }
            else {
                return status_codes::BAD_CMDL_ARGS;
            }
        }
        if (*itr == "-o") {
            if (itr + 1 != cmdl_args.end()) {
                vargs.output_filename = *(++itr);
            }
            else {
                return status_codes::BAD_CMDL_ARGS;
            }
        }
        if (*itr == "-mf") {
            if (itr + 1 != cmdl_args.end()) {
                std::string rational(*(++itr)), num, den;
                num = rational.substr(0, rational.find("/"));
                den = rational.substr(rational.find("/") + 1, std::string::npos);
                if (std::stoi(num) > 0 && std::stoi(den) > 0) {
                    vargs.mod_factor.first  = std::stoi(num);
                    vargs.mod_factor.second = std::stoi(den);
                }
                else {
                    return status_codes::BAD_CMDL_ARGS;
                }
            }
            else {
                return status_codes::BAD_CMDL_ARGS;
            }
        }
    }
    if (vargs.output_filename == "" && vargs.sel_effect == ROBOT) {
        auto pos = vargs.input_filename.find(".");
        vargs.output_filename = vargs.input_filename;
        vargs.output_filename = vargs.output_filename.insert(pos, "_robotized");
    }
    else if (vargs.output_filename == "") {
        auto pos = vargs.input_filename.find(".");
        vargs.output_filename = vargs.input_filename;
        vargs.output_filename = vargs.output_filename.insert(pos, "_tsm");
    }
    // Catch unset modfactor for effects that require it
    if (vargs.sel_effect != ROBOT && vargs.sel_effect != AUTO_TUNE && 
        (!vargs.mod_factor.first || !vargs.mod_factor.second)) 
    {
        std::cout <<  "Please specify a rational modfactor to be used with chosen effect\n";
        std::cout << util::help_msg;
        return status_codes::BAD_CMDL_ARGS;
    }
    return status_codes::SUCCESS;
}

voc_effect util::effect_as_enum(const std::string_view &effect) {
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