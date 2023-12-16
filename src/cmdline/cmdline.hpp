#include "status_codes.hpp"
#include <string>

/* Utility functions for basic parsing of commandline arguments */
namespace util {
    // Parse input argument for chosen effect and modification factor
    status_codes parse_args(const int argc, char *argv[]);
    int effect_as_int(const std::string &effect);
}