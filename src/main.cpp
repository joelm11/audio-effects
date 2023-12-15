#include <iostream>
#include "cmdline/cmdline.hpp"

int main(int argc, char *argv[]) {
    util::parse_args(argc, argv);
    return 0;
}