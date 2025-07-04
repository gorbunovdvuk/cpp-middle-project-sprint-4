#include "metric_impl/cyclomatic_complexity.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using Parameters = std::tuple<std::string, MetricResult::ValueType>;

class CyclomaticComplexityMetricTest : public ::testing::TestWithParam<Parameters> {};

TEST_P(CyclomaticComplexityMetricTest, CheckAllFiles) {
    const auto& [path, expected] = GetParam();
    auto function = function::FunctionExtractor::Get(file::File{std::filesystem::path(TEST_SOURCE_DIR) / path}).front();
    EXPECT_DOUBLE_EQ(CyclomaticComplexityMetric{}.Calculate(function).value, expected);
}

INSTANTIATE_TEST_SUITE_P(
    CheckAllFiles,
    CyclomaticComplexityMetricTest,
    ::testing::Values(
        Parameters{"files/comments.py", 1},
        Parameters{"files/if.py", 2},
        Parameters{"files/many_lines.py", 2},
        Parameters{"files/match_case.py", 5},
        Parameters{"files/simple.py", 2},
        Parameters{"files/exceptions.py", 5},
        Parameters{"files/loops.py", 4},
        Parameters{"files/many_parameters.py", 2},
        Parameters{"files/nested_if.py", 6},
        Parameters{"files/ternary.py", 3}
    )
);

}  // namespace analyser::metric::metric_impl
