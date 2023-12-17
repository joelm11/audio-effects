#include <iostream>
#include "status_codes.hpp"
#include "cmdline/cmdline.hpp"
#include "vocoder/vocoder.hpp"
#include "vocoder_types.hpp"

using status = util::status_codes;

int main(int argc, char *argv[]) {
    
    voc_args vargs;

    if (util::parse_args(argc, argv, vargs) == status::ERROR) {
        std::cout << "Failed to parse commandline arguments\n";
        return static_cast<int>(status::ERROR);
    }
    
    // vocoder<float, 1024> pvc(vocoder<>::effect(0), "samples/speech.wav", "samples/out_speech.wav");
    
    return static_cast<int>(status::SUCCESS);
}