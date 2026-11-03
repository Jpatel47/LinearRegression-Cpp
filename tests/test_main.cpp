// Runs every TEST_CASE registered through test_harness.hpp and prints the
// results. Exits with a non-zero status if any check failed or a test threw.

#include "test_harness.hpp"

#include <cstddef>
#include <exception>
#include <iostream>
#include <string>

int main() {
    std::size_t failed = 0;
    const auto& tests = test_harness::registry();

    for (const auto& test : tests) {
        auto& failures = test_harness::currentFailures();
        failures.clear();
        try {
            test.function();
        } catch (const std::exception& error) {
            failures.push_back(std::string("unexpected exception: ") + error.what());
        } catch (...) {
            failures.push_back("unexpected non-standard exception");
        }

        if (failures.empty()) {
            std::cout << "[PASS] " << test.name << '\n';
        } else {
            ++failed;
            std::cout << "[FAIL] " << test.name << '\n';
            for (const auto& failure : failures) {
                std::cout << "       " << failure << '\n';
            }
        }
    }

    std::cout << '\n' << (tests.size() - failed) << " of " << tests.size() << " tests passed\n";
    return failed == 0 ? 0 : 1;
}
