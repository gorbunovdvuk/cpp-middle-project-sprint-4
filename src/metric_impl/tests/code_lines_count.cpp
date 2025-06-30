#include "metric_impl/code_lines_count.hpp"

#include <filesystem>
#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

TEST(MetricTest, CodeLinesCountMetricTest) {
    static const std::filesystem::path path = "files/comments.py";
    auto function = function::FunctionExtractor::Get(file::File{path}).front();
    EXPECT_DOUBLE_EQ(CodeLinesCountMetric{}.Calculate(function).value, 4);
}

}  // namespace analyser::metric::metric_impl
