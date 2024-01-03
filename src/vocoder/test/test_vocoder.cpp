#include <catch2/catch_test_macros.hpp>
#include "vocoder.hpp"
#include "vocoder_types.hpp"

voc_args vargs = { std::string(""), std::string(""), voc_effect::ROBOT, std::pair<int, int>(1,1) };

TEST_CASE("Test Constructor") {
    // vocoder pvc(vargs);
}