#include <algorithm>
#include <functional>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
#include "metric_accumulator_impl/accumulators.hpp"

namespace analyser {

namespace rv = std::ranges::views;
namespace rs = std::ranges;

using AnalysisResults = std::vector<std::pair<function::Function, metric::MetricResults>>;

AnalysisResults AnalyseFunctions(const std::vector<std::string>& files,  const metric::MetricExtractor& metric_extractor) {
    return files |
        std::views::transform([](const std::string& file) { return file::File(file); }) |
        std::views::transform([](const file::File& file) { return function::FunctionExtractor::Get(file); }) |
        std::views::join |
        std::views::transform([&metric_extractor](const function::Function& function) { return std::make_pair(function, metric_extractor.Get(function)); }) |
        std::ranges::to<std::vector>();
}

std::vector<std::pair<std::string, AnalysisResults>> SplitByClasses(const AnalysisResults& analysis) {
    return {std::from_range, std::ranges::fold_left(analysis, std::unordered_map<std::string, AnalysisResults>{}, [](auto&& map, const auto& analysis) {
        const auto& [function, metrics] = analysis;
        if (function.class_name.has_value()) {
            map[*function.class_name].emplace_back(function, metrics);
        }
        return std::forward<decltype(map)>(map);
    })};
}

std::vector<std::pair<std::string, AnalysisResults>> SplitByFiles(const AnalysisResults& analysis) {
    return {std::from_range, std::ranges::fold_left(analysis, std::unordered_map<std::string, AnalysisResults>{}, [](auto&& map, const auto& analysis) {
        const auto& [function, metrics] = analysis;
        map[function.filename].emplace_back(function, metrics);
        return std::forward<decltype(map)>(map);
    })};
}

void AccumulateFunctionAnalysis(const AnalysisResults &analysis, metric_accumulator::MetricsAccumulator &accumulator) {
    accumulator.ResetAccumulators();
    std::ranges::for_each(analysis | std::views::values, [&accumulator](const auto& metrics) {
        accumulator.AccumulateNextFunctionResults(metrics);
    });
}

} // namespace analyser
