#include <iostream>
#include "cmdline/cmdline.hpp"
#include "vocoder/vocoder.hpp"

int main(int argc, char *argv[]) {
    util::parse_args(argc, argv);
    vocoder<float, 1024> pvc;
    return 0;
}