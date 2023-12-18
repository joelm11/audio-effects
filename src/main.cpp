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

    // While full frames in input run vocoder algorithm.
    while (pvc.analysis() == status::BUFFER_FULL) {
        // if (pvc.user_args.sel_effect == ROBOT) {
        //     pvc.modify_phase_r();
        // }
        // else {
        //     pvc.modify_phase_t();
        // }
        pvc.resynthesis();
    }

    return EXIT_SUCCESS;
}