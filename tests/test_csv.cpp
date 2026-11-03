#include "utils.hpp"
#include "test_harness.hpp"

#include <stdexcept>
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

// Regression: a non-numeric row used to abort the program with an uncaught
// std::invalid_argument thrown by std::stod.
TEST_CASE("readCSV reports a non-numeric row with its line number") {
    const test_harness::TempFile file("x,y\n1,2\nabc,4\n3,6\n");
    std::vector<double> x;
    std::vector<double> y;
    try {
        readCSV(file.path(), x, y);
        FAIL("readCSV did not throw");
    } catch (const std::runtime_error& error) {
        CHECK(test_harness::contains(error.what(), ":3:"));
        CHECK(test_harness::contains(error.what(), "abc,4"));
    }
}

TEST_CASE("readCSV rejects missing values, trailing characters and extra columns") {
    for (const char* contents : {"x,y\n1,2\n4\n", "x,y\n1,2\n4,\n", "x,y\n4abc,5\n", "x,y\n1,2,3\n"}) {
        const test_harness::TempFile file(contents);
        std::vector<double> x;
        std::vector<double> y;
        CHECK_THROWS_AS(readCSV(file.path(), x, y), std::runtime_error);
    }
}

TEST_CASE("readCSV accepts CRLF endings, blank lines, spaces and no header") {
    const test_harness::TempFile file("1,2\r\n\r\n2,4\r\n 3 , 6 \r\n");
    std::vector<double> x;
    std::vector<double> y;
    readCSV(file.path(), x, y);
    CHECK(x.size() == 3);
    CHECK(y.size() == 3);
    if (y.size() == 3) {
        CHECK_NEAR(y[2], 6.0, kTolerance);
    }
}

TEST_CASE("readCSV returns no rows for a header-only file") {
    const test_harness::TempFile file("x,y\n");
    std::vector<double> x;
    std::vector<double> y;
    readCSV(file.path(), x, y);
    CHECK(x.empty());
    CHECK(y.empty());
}

TEST_CASE("readCSV throws when the file cannot be opened") {
    std::vector<double> x;
    std::vector<double> y;
    CHECK_THROWS_AS(readCSV("no/such/dir/missing.csv", x, y), std::runtime_error);
}
