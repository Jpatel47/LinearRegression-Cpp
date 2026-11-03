cmake_minimum_required(VERSION 3.20)

project(LinearRegression LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings are reported for all project targets. Pass
# -DCMAKE_COMPILE_WARNING_AS_ERROR=ON to turn them into errors.
function(linreg_enable_warnings target)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
  endif()
endfunction()

# Regression model and CSV helper, shared by the app and the tests.
add_library(linreg STATIC
  LinearRegression.cpp
  utils.cpp
)
target_include_directories(linreg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
linreg_enable_warnings(linreg)

add_executable(linear_regression main.cpp)
target_link_libraries(linear_regression PRIVATE linreg)
linreg_enable_warnings(linear_regression)

enable_testing()

add_executable(linreg_tests
  tests/test_main.cpp
  tests/test_linear_regression.cpp
  tests/test_csv.cpp
  tests/test_parse.cpp
)
target_link_libraries(linreg_tests PRIVATE linreg)
linreg_enable_warnings(linreg_tests)

add_test(NAME unit_tests COMMAND linreg_tests)

# Smoke test: the app run from the source directory reads the bundled data.csv.
add_test(NAME app_bundled_data
  COMMAND linear_regression
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)
set_tests_properties(app_bundled_data PROPERTIES
  PASS_REGULAR_EXPRESSION
    "Slope: 2[\r\n]+Intercept: 0[\r\n]+Prediction for x = 6: 12[\r\n]+Mean Squared Error: 0"
)

