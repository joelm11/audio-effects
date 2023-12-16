#include <iostream>
#include "status_codes.hpp"
#include "cmdline/cmdline.hpp"
#include "vocoder/vocoder.hpp"

using status = util::status_codes;

int main(int argc, char *argv[]) {
    
    if (util::parse_args(argc, argv) == status::ERROR) {
        std::cout << "Failed to parse commandline arguments\n";
    }
    
    vocoder<float, 1024> pvc(vocoder<>::effect(0), "samples/speech.wav", "samples/out_speech.wav");
    
    return static_cast<int>(status::SUCCESS);
}