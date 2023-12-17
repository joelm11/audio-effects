#pragma once
#include "status_codes.hpp"
#include "vocoder_types.hpp"
#include <string>

/* Utility functions for basic parsing of commandline arguments */
namespace util {
    // Parse input argument for chosen effect and modification factor
    status_codes parse_args(const int argc, char *argv[], voc_args &args);
    voc_effect effect_as_enum(const std::string_view &effect);
}