#include "utils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

void readCSV(const std::string& filename, std::vector<double>& x, std::vector<double>& y) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    bool headerSkipped = false;
    while (getline(file, line)) {
        if (!headerSkipped) {
            headerSkipped = true;
            continue;
        }

        std::stringstream ss(line);
        std::string x_val, y_val;
        getline(ss, x_val, ',');
        getline(ss, y_val, ',');

        x.push_back(std::stod(x_val));
        y.push_back(std::stod(y_val));
    }
    file.close();
}
