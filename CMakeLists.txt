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

# Command-line behaviour: a custom data path and x value.
add_test(NAME app_custom_path_and_x
  COMMAND linear_regression "${PROJECT_SOURCE_DIR}/data.csv" 7
)
set_tests_properties(app_custom_path_and_x PROPERTIES
  PASS_REGULAR_EXPRESSION "Prediction for x = 7: 14"
)

# Invalid data must produce a clear error message and a non-zero exit code.
foreach(case IN ITEMS malformed constant_x)
  add_test(NAME app_${case}_exit_code
    COMMAND linear_regression "${PROJECT_SOURCE_DIR}/tests/data/${case}.csv"
  )
  set_tests_properties(app_${case}_exit_code PROPERTIES WILL_FAIL TRUE)
  add_test(NAME app_${case}_message
    COMMAND linear_regression "${PROJECT_SOURCE_DIR}/tests/data/${case}.csv"
  )
endforeach()
set_tests_properties(app_malformed_message PROPERTIES
  PASS_REGULAR_EXPRESSION "Error: .*malformed\\.csv:3: expected two numbers"
)
set_tests_properties(app_constant_x_message PROPERTIES
  PASS_REGULAR_EXPRESSION "Error: train: all x values are equal"
)
