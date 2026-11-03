#include "utils.hpp"
#include "test_harness.hpp"

#include <vector>

namespace {
constexpr double kTolerance = 1e-12;
}

TEST_CASE("readCSV skips the header row and reads x,y pairs") {
    const test_harness::TempFile file("x,y\n1,2\n2.5,4.75\n-3,-6\n");
    std::vector<double> x;
    std::vector<double> y;
    readCSV(file.path(), x, y);
    CHECK(x.size() == 3);
    CHECK(y.size() == 3);
    if (x.size() == 3 && y.size() == 3) {
        CHECK_NEAR(x[1], 2.5, kTolerance);
        CHECK_NEAR(y[1], 4.75, kTolerance);
        CHECK_NEAR(x[2], -3.0, kTolerance);
        CHECK_NEAR(y[2], -6.0, kTolerance);
    }
}

TEST_CASE("readCSV reads a last row without a trailing newline") {
    const test_harness::TempFile file("x,y\n1,2\n5,10");
    std::vector<double> x;
    std::vector<double> y;
    readCSV(file.path(), x, y);
    CHECK(x.size() == 2);
    if (!y.empty()) {
        CHECK_NEAR(y.back(), 10.0, kTolerance);
    }
}
