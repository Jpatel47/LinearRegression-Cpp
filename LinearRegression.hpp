#ifndef LINEAR_REGRESSION_HPP
#define LINEAR_REGRESSION_HPP

#include <vector>

class LinearRegression {
private:
    double slope;
    double intercept;

public:
    LinearRegression();
    void train(const std::vector<double>& x, const std::vector<double>& y);
    double predict(double x_value) const;
    double meanSquaredError(const std::vector<double>& x, const std::vector<double>& y) const;
    double getSlope() const;
    double getIntercept() const;
};

#endif
