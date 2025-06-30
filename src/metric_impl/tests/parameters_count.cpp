#include "metric_impl/parameters_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {


using Parameters = std::tuple<std::string, MetricResult::ValueType>;

class CountParametersMetricTest : public ::testing::TestWithParam<Parameters> {};

TEST_P(CountParametersMetricTest, CheckAllFiles) {
    const auto& [path, expected] = GetParam();
    auto function = function::FunctionExtractor::Get(file::File{std::filesystem::path(TEST_SOURCE_DIR) / path}).front();
    EXPECT_DOUBLE_EQ(CountParametersMetric{}.Calculate(function).value, expected);
}

INSTANTIATE_TEST_SUITE_P(
    CheckAllFiles,
    CountParametersMetricTest,
    ::testing::Values(
        Parameters{"files/comments.py", 3},
        Parameters{"files/if.py", 1},
        Parameters{"files/many_lines.py", 0},
        Parameters{"files/match_case.py", 1},
        Parameters{"files/simple.py", 0},
        Parameters{"files/exceptions.py", 0},
        Parameters{"files/loops.py", 1},
        Parameters{"files/many_parameters.py", 7},
        Parameters{"files/nested_if.py", 2},
        Parameters{"files/ternary.py", 1}
    )
);

}  // namespace analyser::metric::metric_impl
