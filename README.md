# Linear Regression in C++

[![CI](https://github.com/Jpatel47/LinearRegression-Cpp/actions/workflows/ci.yml/badge.svg)](https://github.com/Jpatel47/LinearRegression-Cpp/actions/workflows/ci.yml)

This project is a simple implementation of **Linear Regression** from scratch using **C++**. It demonstrates how a model can be trained on numerical data, make predictions, and evaluate accuracy using **Mean Squared Error (MSE)** — all without using external machine learning libraries.

---

## 📂 Project Structure

```
├── LinearRegression.hpp/.cpp   # Core ML logic (training, prediction, error)
├── main.cpp                    # Program entry, reads data and runs model
├── utils.hpp/.cpp              # CSV reading utility
├── data.csv                    # Input training data
├── tests/                      # Unit tests (dependency-free harness, run by CTest)
├── CMakeLists.txt              # CMake build for the app and the tests
├── LinearRegression.sln        # Visual Studio 2019 solution (+ .vcxproj files)
├── .github/workflows/ci.yml    # CI: build and test on Linux, macOS and Windows
├── LICENSE                     # MIT
├── README.md                   # Project documentation
```

---

## 📊 What It Does

- Reads `x,y` data from a CSV file
- Calculates slope and intercept using **least squares method**
- Predicts `y` for a given `x` using the trained model
- Computes **mean squared error** to evaluate model accuracy

---

## 📁 Example Input (`data.csv`)

```csv
x,y
1,2
2,4
3,6
4,8
5,10
```

---

## 🚀 How to Run

### 🧱 Compile:

```bash
g++ main.cpp LinearRegression.cpp utils.cpp -o linear_regression
```

### ▶ Run:

```bash
./linear_regression
```

---

## 🧪 Build and Test with CMake

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The tests use a small dependency-free harness (`tests/test_harness.hpp`) whose checks
stay active in Release builds. CTest also runs the app against the bundled `data.csv`
and checks its output. GitHub Actions runs the same steps on Linux, macOS and Windows.

---

## 🧠 Concepts Covered

- Linear Regression (Supervised ML)
- Mean Squared Error
- C++ STL usage (vectors, accumulate, file I/O)
- Basic ML logic implementation from scratch
- CSV parsing in C++

---

## 🧑‍💻 Author

**Jay Patel**  
📧 jpatel3847@gmail.com  
🔗 [LinkedIn](https://www.linkedin.com/in/jaypatel3847)

The CMake build, tests and CI were added with AI pair-programming assistance (Claude Code).

---

## ⭐️ Future Improvements

- Support multiple features (multi-variate regression)
- Add command-line options for input files and predictions
- Plot predictions vs actual (using Python or gnuplot)

---

## 📄 License

MIT. See [LICENSE](LICENSE).
