#include <cstdlib>
#include <iostream>
#include "status_codes.hpp"
#include "cmdline/cmdline.hpp"
#include "vocoder/vocoder.hpp"
#include "vocoder_types.hpp"

using status  = util::status_codes;

int main(int argc, char *argv[]) {
    
    voc_args vargs;

    if (util::parse_args(argc, argv, vargs) != status::SUCCESS) {
        std::cout << "Bad commandline arguments given\n";
        return EXIT_FAILURE;
    }
    
    vocoder pvc(vargs);
    
    if (pvc.vocoder_init() != status::SUCCESS) {
        std::cout << "Failed to initialize phase vocoder\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}