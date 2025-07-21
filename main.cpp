#include "LinearRegression.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

int main() {
    std::vector<double> x_values, y_values;

    std::ifstream file("data.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    std::string line;
    bool skip_header = true;
    while (std::getline(file, line)) {
        if (skip_header) {
            skip_header = false;
            continue;
        }

        std::stringstream ss(line);
        std::string x_str, y_str;

        if (std::getline(ss, x_str, ',') && std::getline(ss, y_str, ',')) {
            double x = std::stod(x_str);
            double y = std::stod(y_str);
            x_values.push_back(x);
            y_values.push_back(y);
        }
    }

    file.close();

    LinearRegression model;
    model.train(x_values, y_values);

    std::cout << "Trained Model:\n";
    std::cout << "Slope: " << model.getSlope() << std::endl;
    std::cout << "Intercept: " << model.getIntercept() << std::endl;

    double input_x = 6;
    std::cout << "Prediction for x = " << input_x << ": " << model.predict(input_x) << std::endl;
    std::cout << "Mean Squared Error: " << model.meanSquaredError(x_values, y_values) << std::endl;

    return 0;
}
