#include "metric.hpp"

#include <algorithm>
#include <ranges>
#include <vector>

#include "function.hpp"

namespace analyser::metric {

void MetricExtractor::RegisterMetric(std::unique_ptr<IMetric> metric) {
    metrics.emplace_back(std::move(metric));
}

MetricResults MetricExtractor::Get(const function::Function &func) const {
    return metrics | std::views::transform([&func](const std::unique_ptr<IMetric>& metric) {
        return metric->Calculate(func);
    }) | std::ranges::to<std::vector>();
}

}  // namespace analyser::metric
