#include "metric_impl/parameters_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

TEST(MetricTest, CountParametersMetricTest) {
    static const std::filesystem::path path = "files/many_parameters.py";
    auto function = function::FunctionExtractor::Get(file::File{path}).front();
    EXPECT_DOUBLE_EQ(CountParametersMetric{}.Calculate(function).value, 7);
}

}  // namespace analyser::metric::metric_impl
