#include "LinearRegression.hpp"
#include "utils.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// Usage: linear_regression [data.csv] [x]
//   data.csv  CSV file with x,y rows and an optional header (default: data.csv)
//   x         value to predict y for (default: 6)
// Exit codes: 0 on success, 1 for unreadable or invalid data, 2 for bad arguments.
int main(int argc, char* argv[]) {
    if (argc > 3) {
        std::cerr << "Usage: " << argv[0] << " [data.csv] [x]" << std::endl;
        return 2;
    }
    const std::string path = argc > 1 ? argv[1] : "data.csv";

    double input_x = 6;
    if (argc > 2) {
        try {
            input_x = parseNumber(argv[2]);
        } catch (const std::invalid_argument&) {
            std::cerr << "Error: x must be a number, got '" << argv[2] << "'" << std::endl;
            return 2;
        }
    }

    try {
        std::vector<double> x_values, y_values;
        readCSV(path, x_values, y_values);
        if (x_values.empty()) {
            std::cerr << "Error: " << path << " contains no data rows" << std::endl;
            return 1;
        }

        LinearRegression model;
        model.train(x_values, y_values);

        std::cout << "Trained Model:\n";
        std::cout << "Slope: " << model.getSlope() << std::endl;
        std::cout << "Intercept: " << model.getIntercept() << std::endl;

        std::cout << "Prediction for x = " << input_x << ": " << model.predict(input_x) << std::endl;
        std::cout << "Mean Squared Error: " << model.meanSquaredError(x_values, y_values) << std::endl;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << std::endl;
        return 1;
    }

    return 0;
}
