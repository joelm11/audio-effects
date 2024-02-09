#pragma once
#include <string>
#include "status_codes.hpp"
#include "vocoder_types.hpp"

/* Utility functions for basic parsing of commandline arguments */
namespace util {
    /* Parse input argument for chosen effect and modification factor */
    status_codes parse_args(const int argc, char *argv[], voc_args &args);

    /* Convert effect string to effect enum */
    voc_effect effect_as_enum(const std::string_view &effect);

    /* Help / Error string */
    const std::string help_msg ("effects_app <infile.wav> [options]\n"
        "Options:\n"
        "-o <outfile.wav> = Specify an output file\n"
        "-e = Specify an effect from one of 'robot', 'tstretch', 'pitch'\n"
        "-mf <num/den> = Modification factor as a positive rational\n"
        );
}