#include "utils.hpp"
#include <cmath>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace {

std::string trim(const std::string& text) {
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const auto last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

bool tryParseNumber(const std::string& text, double& value) {
    const std::string trimmed = trim(text);
    if (trimmed.empty()) {
        return false;
    }
    try {
        std::size_t used = 0;
        value = std::stod(trimmed, &used);
        return used == trimmed.size() && std::isfinite(value);
    } catch (const std::exception&) {  // std::invalid_argument or std::out_of_range
        return false;
    }
}

// Splits "x,y" into two numbers. Rejects missing, extra or non-numeric fields.
bool tryParseRow(const std::string& line, double& x, double& y) {
    std::stringstream row(line);
    std::string x_field, y_field, extra;
    if (!std::getline(row, x_field, ',') || !std::getline(row, y_field, ',')) {
        return false;
    }
    if (std::getline(row, extra, ',') && !trim(extra).empty()) {
        return false;
    }
    return tryParseNumber(x_field, x) && tryParseNumber(y_field, y);
}

}  // namespace

double parseNumber(const std::string& text) {
    double value = 0.0;
    if (!tryParseNumber(text, value)) {
        throw std::invalid_argument("not a number: '" + text + "'");
    }
    return value;
}

void readCSV(const std::string& filename, std::vector<double>& x, std::vector<double>& y) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("cannot open " + filename);
    }

    std::string line;
    std::size_t line_number = 0;
    bool first_content_line = true;
    while (std::getline(file, line)) {
        ++line_number;
        if (trim(line).empty()) {
            continue;
        }

        double x_value = 0.0;
        double y_value = 0.0;
        if (tryParseRow(line, x_value, y_value)) {
            x.push_back(x_value);
            y.push_back(y_value);
        } else if (!first_content_line) {
            throw std::runtime_error(filename + ":" + std::to_string(line_number) +
                                     ": expected two numbers 'x,y', got '" + trim(line) + "'");
        }
        first_content_line = false;  // a non-numeric first line is the header
    }
}
