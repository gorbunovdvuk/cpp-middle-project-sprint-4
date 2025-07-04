#include "metric_impl/code_lines_count.hpp"

#include <filesystem>
#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using Parameters = std::tuple<std::string, MetricResult::ValueType>;

class CodeLinesCountMetricTest : public ::testing::TestWithParam<Parameters> {};

TEST_P(CodeLinesCountMetricTest, CheckAllFiles) {
    const auto& [path, expected] = GetParam();
    auto function = function::FunctionExtractor::Get(file::File{std::filesystem::path(TEST_SOURCE_DIR) / path}).front();
    EXPECT_DOUBLE_EQ(CodeLinesCountMetric{}.Calculate(function).value, expected);
}

INSTANTIATE_TEST_SUITE_P(
    CheckAllFiles,
    CodeLinesCountMetricTest,
    ::testing::Values(
        Parameters{"files/comments.py", 4},
        Parameters{"files/if.py", 4},
        Parameters{"files/many_lines.py", 12},
        Parameters{"files/match_case.py", 8},
        Parameters{"files/simple.py", 6},
        Parameters{"files/exceptions.py", 8},
        Parameters{"files/loops.py", 7},
        Parameters{"files/many_parameters.py", 2},
        Parameters{"files/nested_if.py", 9},
        Parameters{"files/ternary.py", 2}
    )
);

}  // namespace analyser::metric::metric_impl
