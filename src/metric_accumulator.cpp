#include "metric_accumulator.hpp"

#include <algorithm>
#include <array>
#include <filesystem>
#include <functional>
#include <iostream>
#include <ranges>
#include <vector>

namespace analyser::metric_accumulator {

void MetricsAccumulator::AccumulateNextFunctionResults(const std::vector<metric::MetricResult> &metric_results) const {
    std::ranges::for_each(metric_results, [this](const metric::MetricResult &result) {
        if (auto it = accumulators_.find(result.metric_name); it != accumulators_.end()) {
            it->second->Accumulate(result);
        }
    });
}
void MetricsAccumulator::ResetAccumulators() {
    std::ranges::for_each(accumulators_ | std::views::values, [](auto &acc) { acc->Reset(); });
}
}  // namespace analyser::metric_accumulator
