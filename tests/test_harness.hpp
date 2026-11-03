#ifndef LINREG_TEST_HARNESS_HPP
#define LINREG_TEST_HARNESS_HPP

// A minimal, dependency-free test harness.
//
//   TEST_CASE("description") { ... }   registers a test function.
//   CHECK(condition)                   fails if condition is false.
//   CHECK_NEAR(actual, expected, tol)  fails if |actual - expected| > tol (or NaN).
//   CHECK_THROWS_AS(expression, type)  fails unless expression throws `type`.
//   FAIL(message)                      records an unconditional failure.
//
// A failed check is recorded and the test keeps running. The checks are plain
// if-statements, so they work in every build type (assert() is compiled out
// when NDEBUG is defined). The runner in test_main.cpp returns a non-zero exit
// code if any check fails or a test throws an exception.

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

namespace test_harness {

struct TestCase {
    const char* name;
    void (*function)();
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> tests;
    return tests;
}

// Failure messages recorded by the test that is currently running.
inline std::vector<std::string>& currentFailures() {
    static std::vector<std::string> failures;
    return failures;
}

inline void reportFailure(const char* file, int line, const std::string& message) {
    std::ostringstream entry;
    entry << file << ':' << line << ": " << message;
    currentFailures().push_back(entry.str());
}

struct Registrar {
    Registrar(const char* name, void (*function)()) {
        registry().push_back(TestCase{name, function});
    }
};

inline bool contains(const std::string& text, const std::string& part) {
    return text.find(part) != std::string::npos;
}

// Writes `contents` byte-for-byte to a new file in the system temporary
// directory and deletes the file again when the object goes out of scope.
class TempFile {
public:
    explicit TempFile(const std::string& contents) : path_(uniquePath()) {
        std::ofstream out(path_, std::ios::binary);
        out << contents;
        if (!out) {
            throw std::runtime_error("cannot write temporary file " + path_.string());
        }
    }

    ~TempFile() {
        std::error_code ignored;
        std::filesystem::remove(path_, ignored);
    }

    TempFile(const TempFile&) = delete;
    TempFile& operator=(const TempFile&) = delete;

    std::string path() const { return path_.string(); }

private:
    static std::filesystem::path uniquePath() {
        static std::mt19937 random{std::random_device{}()};
        static unsigned counter = 0;
        const std::string name = "linreg_test_" + std::to_string(random()) + "_" +
                                 std::to_string(++counter) + ".csv";
        return std::filesystem::temp_directory_path() / name;
    }

    std::filesystem::path path_;
};

}  // namespace test_harness

#define TEST_HARNESS_CONCAT_(a, b) a##b
#define TEST_HARNESS_CONCAT(a, b) TEST_HARNESS_CONCAT_(a, b)

#define TEST_HARNESS_CASE_(name, fn)                                                          \
    static void fn();                                                                         \
    static const ::test_harness::Registrar TEST_HARNESS_CONCAT(fn, _registrar)(name, &fn);    \
    static void fn()

#define TEST_CASE(name) TEST_HARNESS_CASE_(name, TEST_HARNESS_CONCAT(test_case_, __LINE__))

#define FAIL(message) ::test_harness::reportFailure(__FILE__, __LINE__, (message))

#define CHECK(condition)                                                                      \
    do {                                                                                      \
        if (!(condition)) {                                                                   \
            ::test_harness::reportFailure(__FILE__, __LINE__, "CHECK(" #condition ") failed"); \
        }                                                                                     \
    } while (false)

#define CHECK_NEAR(actual, expected, tolerance)                                               \
    do {                                                                                      \
        const double check_actual_ = (actual);                                                \
        const double check_expected_ = (expected);                                            \
        if (!(std::fabs(check_actual_ - check_expected_) <= (tolerance))) {                   \
            std::ostringstream check_message_;                                                \
            check_message_ << std::setprecision(17) << "CHECK_NEAR(" #actual ", " #expected   \
                           << ", " #tolerance ") failed: got " << check_actual_               \
                           << ", expected " << check_expected_;                               \
            ::test_harness::reportFailure(__FILE__, __LINE__, check_message_.str());          \
        }                                                                                     \
    } while (false)

#define CHECK_THROWS_AS(expression, exception_type)                                           \
    do {                                                                                      \
        std::string check_problem_ = "did not throw";                                         \
        try {                                                                                 \
            static_cast<void>(expression);                                                    \
        } catch (const exception_type&) {                                                     \
            check_problem_.clear();                                                           \
        } catch (const std::exception& check_other_) {                                        \
            check_problem_ = std::string("threw a different exception: ") + check_other_.what(); \
        } catch (...) {                                                                       \
            check_problem_ = "threw a non-standard exception";                                \
        }                                                                                     \
        if (!check_problem_.empty()) {                                                        \
            ::test_harness::reportFailure(__FILE__, __LINE__,                                 \
                "CHECK_THROWS_AS(" #expression ", " #exception_type ") failed: " + check_problem_); \
        }                                                                                     \
    } while (false)

#endif
