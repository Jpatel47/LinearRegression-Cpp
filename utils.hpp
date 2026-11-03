#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

// Reads two numeric columns (x,y) from a CSV file and appends them to x and y.
// If the first non-blank line is not numeric it is treated as a header and
// skipped. Blank lines are ignored and Windows (CRLF) line endings are accepted.
// Throws std::runtime_error if the file cannot be opened or a row is not exactly
// two numbers; the message names the file and the line number.
void readCSV(const std::string& filename, std::vector<double>& x, std::vector<double>& y);

// Parses the whole string (surrounding whitespace allowed) as a finite number.
// Throws std::invalid_argument if anything else remains, e.g. "4abc" or "nan".
double parseNumber(const std::string& text);

#endif
