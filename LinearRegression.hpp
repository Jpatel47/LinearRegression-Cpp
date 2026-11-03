#ifndef LINEAR_REGRESSION_HPP
#define LINEAR_REGRESSION_HPP

#include <vector>

class LinearRegression {
private:
    double slope;
    double intercept;

public:
    LinearRegression();
    // Fits y = slope * x + intercept by ordinary least squares.
    // Throws std::invalid_argument if x is empty or x and y differ in length,
    // and std::domain_error if all x values are equal (the slope is undefined).
    // On error the previous fit is kept.
    void train(const std::vector<double>& x, const std::vector<double>& y);
    double predict(double x_value) const;
    // Throws std::invalid_argument if x is empty or x and y differ in length.
    double meanSquaredError(const std::vector<double>& x, const std::vector<double>& y) const;
    double getSlope() const;
    double getIntercept() const;
};

#endif
