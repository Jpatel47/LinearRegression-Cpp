# Linear Regression in C++

This project is a simple implementation of **Linear Regression** from scratch using **C++**. It demonstrates how a model can be trained on numerical data, make predictions, and evaluate accuracy using **Mean Squared Error (MSE)** — all without using external machine learning libraries.

---

## 📂 Project Structure

```
├── LinearRegression.hpp/.cpp   # Core ML logic (training, prediction, error)
├── main.cpp                    # Program entry, reads data and runs model
├── utils.hpp/.cpp              # CSV reading utility
├── data.csv                    # Input training data
├── .gitignore                  # Ignores VS/IDE build files
├── README.md                   # Project documentation
```

---

## 📊 What It Does

- Reads `X,Y` data from a CSV file
- Calculates slope and intercept using **least squares method**
- Predicts `Y` for a given `X` using the trained model
- Computes **mean squared error** to evaluate model accuracy

---

## 📁 Example Input (`data.csv`)

```csv
X,Y
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

---

## ⭐️ Future Improvements

- Support multiple features (multi-variate regression)
- Add command-line options for input files and predictions
- Plot predictions vs actual (using Python or gnuplot)
