#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace analyser::metric_accumulator::metric_accumulator_impl {

void SumAverageAccumulator::Accumulate(const metric::MetricResult& metric_result) {
    if (is_finalized) {
        throw std::runtime_error("SumAverageAccumulator is finalized");
    }
    sum += metric_result.value;
    count++;
}

void SumAverageAccumulator::Finalize() {
    if (count > 0) {
        average = static_cast<double>(sum) / count;
    }
    is_finalized = true;
}

void SumAverageAccumulator::Reset() {
    average = sum = count = 0;
    is_finalized = false;
}

SumAverageAccumulator::SumAverage SumAverageAccumulator::Get() const {
    if (!is_finalized) {
        throw std::runtime_error("Accumulator is not finalized");
    }
    return {sum, average};
}

// здесь ваш код

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
