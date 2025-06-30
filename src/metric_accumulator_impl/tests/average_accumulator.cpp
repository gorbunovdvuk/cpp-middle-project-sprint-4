#include "metric_accumulator_impl/average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(AverageAccumulatorTest, EmptyTest) {
    AverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 0);
}

TEST(AverageAccumulatorTest, NumbersTest) {
    AverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 2});
    accumulator.Accumulate(metric::MetricResult{"", 3});
    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 2.5);
}

TEST(AverageAccumulatorTest, MustBeFinalizedTest) {
    AverageAccumulator accumulator;
    accumulator.Reset();
    EXPECT_THROW(accumulator.Get(), std::runtime_error);
}

TEST(AverageAccumulatorTest, AccumulateAfterFinalizeTest) {
    AverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Finalize();
    EXPECT_THROW(accumulator.Accumulate(metric::MetricResult{"", 0}), std::runtime_error);
}

TEST(AverageAccumulatorTest, ComputeAfterResetTest) {
    AverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 3});
    accumulator.Accumulate(metric::MetricResult{"", 6});
    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 4.5);
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 1});
    accumulator.Accumulate(metric::MetricResult{"", 3});
    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 2);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
