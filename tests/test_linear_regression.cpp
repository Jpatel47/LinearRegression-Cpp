#include "LinearRegression.hpp"
#include "test_harness.hpp"

#include <stdexcept>
#include <vector>

namespace {
constexpr double kTolerance = 1e-9;
}

TEST_CASE("fits an exact line y = 2x") {
    const std::vector<double> x{1, 2, 3, 4, 5};
    const std::vector<double> y{2, 4, 6, 8, 10};
    LinearRegression model;
    model.train(x, y);
    CHECK_NEAR(model.getSlope(), 2.0, kTolerance);
    CHECK_NEAR(model.getIntercept(), 0.0, kTolerance);
    CHECK_NEAR(model.predict(6), 12.0, kTolerance);
    CHECK_NEAR(model.meanSquaredError(x, y), 0.0, kTolerance);
}

// The expected values in the next two tests were computed independently with
// exact rational arithmetic (Python's fractions module), not with this code.
TEST_CASE("fits noisy data") {
    const std::vector<double> x{1, 2, 3, 4, 5};
    const std::vector<double> y{1.9, 3.7, 6.2, 7.9, 10.1};
    LinearRegression model;
    model.train(x, y);
    CHECK_NEAR(model.getSlope(), 2.06, kTolerance);
    CHECK_NEAR(model.getIntercept(), -0.22, kTolerance);
    CHECK_NEAR(model.predict(7), 14.2, kTolerance);
    CHECK_NEAR(model.meanSquaredError(x, y), 0.0232, kTolerance);
}

TEST_CASE("fits a negative slope from unsorted, non-integer x") {
    const std::vector<double> x{2, -3, 6, 0.5, -1};
    const std::vector<double> y{-1.9, 7.5, -10.4, 1.2, 4.1};
    LinearRegression model;
    model.train(x, y);
    CHECK_NEAR(model.getSlope(), -2.0054112554112553, kTolerance);
    CHECK_NEAR(model.getIntercept(), 1.9048701298701298, kTolerance);
    CHECK_NEAR(model.meanSquaredError(x, y), 0.08372943722943722, kTolerance);
}

TEST_CASE("an untrained model predicts zero") {
    const LinearRegression model;
    CHECK_NEAR(model.getSlope(), 0.0, kTolerance);
    CHECK_NEAR(model.getIntercept(), 0.0, kTolerance);
    CHECK_NEAR(model.predict(42), 0.0, kTolerance);
}

// Regression: all-equal x values used to divide by zero and print "nan".
TEST_CASE("rejects data where every x is the same") {
    const std::vector<double> x{3, 3, 3};
    const std::vector<double> y{1, 2, 3};
    LinearRegression model;
    CHECK_THROWS_AS(model.train(x, y), std::domain_error);
}

TEST_CASE("a failed train keeps the previous fit") {
    const std::vector<double> x{1, 2, 3};
    const std::vector<double> y{2, 4, 6};
    const std::vector<double> same_x{5, 5};
    const std::vector<double> two_y{1, 2};
    LinearRegression model;
    model.train(x, y);
    CHECK_THROWS_AS(model.train(same_x, two_y), std::domain_error);
    CHECK_NEAR(model.getSlope(), 2.0, kTolerance);
    CHECK_NEAR(model.getIntercept(), 0.0, kTolerance);
}

TEST_CASE("rejects empty or mismatched input") {
    const std::vector<double> empty;
    const std::vector<double> two{1, 2};
    const std::vector<double> one{1};
    LinearRegression model;
    CHECK_THROWS_AS(model.train(empty, empty), std::invalid_argument);
    CHECK_THROWS_AS(model.train(two, one), std::invalid_argument);
    CHECK_THROWS_AS(model.meanSquaredError(empty, empty), std::invalid_argument);
    CHECK_THROWS_AS(model.meanSquaredError(two, one), std::invalid_argument);
}
