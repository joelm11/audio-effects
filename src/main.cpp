#include <cstdlib>
#include <iostream>
#include "status_codes.hpp"
#include "cmdline.hpp"
#include "vocoder.hpp"
#include "vocoder_types.hpp"
#include "resample/inc/resample.hpp"

using status  = util::status_codes;

int main(int argc, char *argv[]) {
    
    const int FRAME_SIZE = 1024;

    voc_args vargs;

    if (util::parse_args(argc, argv, vargs) != status::SUCCESS) {
        std::cout << util::help_msg;
        std::cout << "Bad commandline arguments given. Terminating.\n";
        return EXIT_FAILURE;
    }
    
    vocoder pvc(vargs, FRAME_SIZE);
    
    if (pvc.vocoder_init() != status::SUCCESS) {
        std::cout << "Failed to initialize phase vocoder. Terminating.\n";
        return EXIT_FAILURE;
    }

    // While full frames in input run vocoder algorithm.
    while (pvc.analysis() == status::BUFFER_FULL) {
        if (pvc.user_args.sel_effect == ROBOT) {
            pvc.modify_phase_r();
        }
        else {
            pvc.modify_phase_t();
        }
        pvc.resynthesis();
    }
    
    if (pvc.user_args.sel_effect == PITCH_SHIFT || pvc.user_args.sel_effect == AUTO_TUNE) {
        resampler<double, 13, 512> rsmp;
        auto pos = vargs.input_filename.find(".");
        auto pitch_fname = vargs.output_filename.insert(pos, "_pitched");
        rsmp.resampler_init(pvc.user_args.output_filename, pitch_fname, pvc.frame_size);
        int fs_new = pvc.file_data.samplerate * (static_cast<double>(pvc.user_args.mod_factor.second) 
            / pvc.user_args.mod_factor.first
        );
        if (pvc.user_args.sel_effect == PITCH_SHIFT) {
            rsmp.resample(pvc.file_data.samplerate, fs_new);
        }
        else {
            rsmp.resample_varying(pvc.file_data.samplerate, pvc.alphas);
        }
    }

    return EXIT_SUCCESS;
}