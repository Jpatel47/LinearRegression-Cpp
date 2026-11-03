#include "utils.hpp"
#include "test_harness.hpp"

#include <stdexcept>

TEST_CASE("parseNumber accepts a whole number with surrounding spaces") {
    CHECK_NEAR(parseNumber(" 2.5 "), 2.5, 1e-12);
    CHECK_NEAR(parseNumber("-1e3"), -1000.0, 1e-12);
}

TEST_CASE("parseNumber rejects partial, empty and non-finite input") {
    CHECK_THROWS_AS(parseNumber("2.5x"), std::invalid_argument);
    CHECK_THROWS_AS(parseNumber(""), std::invalid_argument);
    CHECK_THROWS_AS(parseNumber("abc"), std::invalid_argument);
    CHECK_THROWS_AS(parseNumber("nan"), std::invalid_argument);
    CHECK_THROWS_AS(parseNumber("1e999"), std::invalid_argument);
}
