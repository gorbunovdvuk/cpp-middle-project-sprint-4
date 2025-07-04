#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(SumAverageAccumulatorTest, EmptyTest) {
    SumAverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Finalize();
    EXPECT_TRUE(accumulator.Get() == SumAverageAccumulator::SumAverage(0, 0));
}

TEST(SumAverageAccumulatorTest, NumbersTest) {
    SumAverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 11});
    accumulator.Accumulate(metric::MetricResult{"", 37});
    accumulator.Finalize();
    EXPECT_TRUE(accumulator.Get() == SumAverageAccumulator::SumAverage(48, 24));
}

TEST(SumAverageAccumulatorTest, MustBeFinalizedTest) {
    SumAverageAccumulator accumulator;
    accumulator.Reset();
    EXPECT_THROW(accumulator.Get(), std::runtime_error);
}

TEST(SumAverageAccumulatorTest, AccumulateAfterFinalizeTest) {
    SumAverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Finalize();
    EXPECT_THROW(accumulator.Accumulate(metric::MetricResult{"", 0}), std::runtime_error);
}

TEST(SumAverageAccumulatorTest, ComputeAfterResetTest) {
    SumAverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 93});
    accumulator.Accumulate(metric::MetricResult{"", 12});
    accumulator.Finalize();
    EXPECT_TRUE(accumulator.Get() == SumAverageAccumulator::SumAverage(105, 52.5));
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 17});
    accumulator.Accumulate(metric::MetricResult{"", 81});
    accumulator.Finalize();
    EXPECT_TRUE(accumulator.Get() == SumAverageAccumulator::SumAverage(98, 49));
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
