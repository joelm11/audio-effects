#include <string>

/* Utility functions for basic parsing of commandline arguments */
namespace util {
    // Parse input argument for chosen effect and modification factor
    int parse_args(const int argc, char *argv[]);
    int effect_as_int(const std::string &effect);
}