#include <iostream>
#include "cmdline/cmdline.hpp"

int main() {
    std::cout << "Been a while\n";
    util::parse_args();
    return 0;
}