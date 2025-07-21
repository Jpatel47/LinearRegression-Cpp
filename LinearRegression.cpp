#include "LinearRegression.hpp"
#include <numeric>  // for accumulate
#include <cmath>    // for pow

LinearRegression::LinearRegression() : slope(0), intercept(0) {}

void LinearRegression::train(const std::vector<double>& x, const std::vector<double>& y) {
    if (x.size() != y.size() || x.empty()) return;

    double x_mean = std::accumulate(x.begin(), x.end(), 0.0) / x.size();
    double y_mean = std::accumulate(y.begin(), y.end(), 0.0) / y.size();

    double numerator = 0.0;
    double denominator = 0.0;

    for (size_t i = 0; i < x.size(); ++i) {
        numerator += (x[i] - x_mean) * (y[i] - y_mean);
        denominator += std::pow(x[i] - x_mean, 2);
    }

    slope = numerator / denominator;
    intercept = y_mean - slope * x_mean;
}

double LinearRegression::predict(double x_value) const {
    return slope * x_value + intercept;
}

double LinearRegression::meanSquaredError(const std::vector<double>& x, const std::vector<double>& y) const {
    if (x.size() != y.size() || x.empty()) return 0.0;

    double mse = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        double prediction = predict(x[i]);
        mse += std::pow(prediction - y[i], 2);
    }
    return mse / x.size();
}

double LinearRegression::getSlope() const {
    return slope;
}

double LinearRegression::getIntercept() const {
    return intercept;
}
