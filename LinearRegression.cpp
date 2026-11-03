#include "LinearRegression.hpp"
#include <numeric>  // for accumulate
#include <cmath>    // for pow
#include <stdexcept>
#include <string>

namespace {

void requireMatchingData(const std::vector<double>& x, const std::vector<double>& y, const char* caller) {
    if (x.empty()) {
        throw std::invalid_argument(std::string(caller) + ": no data points");
    }
    if (x.size() != y.size()) {
        throw std::invalid_argument(std::string(caller) + ": x and y have different lengths");
    }
}

}  // namespace

LinearRegression::LinearRegression() : slope(0), intercept(0) {}

void LinearRegression::train(const std::vector<double>& x, const std::vector<double>& y) {
    requireMatchingData(x, y, "train");

    const double n = static_cast<double>(x.size());
    double x_mean = std::accumulate(x.begin(), x.end(), 0.0) / n;
    double y_mean = std::accumulate(y.begin(), y.end(), 0.0) / n;

    double numerator = 0.0;
    double denominator = 0.0;

    for (size_t i = 0; i < x.size(); ++i) {
        numerator += (x[i] - x_mean) * (y[i] - y_mean);
        denominator += std::pow(x[i] - x_mean, 2);
    }

    if (denominator == 0.0) {
        throw std::domain_error("train: all x values are equal, so the slope is undefined");
    }

    slope = numerator / denominator;
    intercept = y_mean - slope * x_mean;
}

double LinearRegression::predict(double x_value) const {
    return slope * x_value + intercept;
}

double LinearRegression::meanSquaredError(const std::vector<double>& x, const std::vector<double>& y) const {
    requireMatchingData(x, y, "meanSquaredError");

    double mse = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        double prediction = predict(x[i]);
        mse += std::pow(prediction - y[i], 2);
    }
    return mse / static_cast<double>(x.size());
}

double LinearRegression::getSlope() const {
    return slope;
}

double LinearRegression::getIntercept() const {
    return intercept;
}
