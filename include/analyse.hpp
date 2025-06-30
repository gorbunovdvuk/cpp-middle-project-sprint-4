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
    std::unordered_map<std::string, AnalysisResults> classes;
    for (const auto& [function, metrics] : analysis) {
        if (function.class_name.has_value()) {
            classes[*function.class_name].emplace_back(function, metrics);
        }
    }
    return {std::from_range, classes};
}

std::vector<std::pair<std::string, AnalysisResults>> SplitByFiles(const AnalysisResults& analysis) {
    std::unordered_map<std::string, AnalysisResults> files;
    for (const auto& [function, metrics] : analysis) {
        files[function.filename].emplace_back(function, metrics);
    }
    return {std::from_range, files};
}

void AccumulateFunctionAnalysis(const AnalysisResults& analysis, metric_accumulator::MetricsAccumulator& accumulator, size_t indent = 4) {
    accumulator.ResetAccumulators();
    std::ranges::for_each(analysis | std::views::values, [&accumulator](const auto& metrics) {
        accumulator.AccumulateNextFunctionResults(metrics);
    });
    const auto code_lines = accumulator.GetFinalizedAccumulator<metric_accumulator::metric_accumulator_impl::SumAverageAccumulator>("code_lines");
    const auto parameters = accumulator.GetFinalizedAccumulator<metric_accumulator::metric_accumulator_impl::AverageAccumulator>("parameters");
    const auto complexity = accumulator.GetFinalizedAccumulator<metric_accumulator::metric_accumulator_impl::SumAverageAccumulator>("complexity");

    std::vector<std::pair<std::string_view, double>> results{
        {"code_lines_sum", code_lines.Get().sum},
        {"code_lines_avg", code_lines.Get().average},
        {"parameters_avg", parameters.Get()},
        {"complexity_sum", complexity.Get().sum},
        {"complexity_avg", complexity.Get().average},
    };

    for (const auto& [name, value] : results) {
        std::print("{}{}: {}\n", std::string(indent, ' '), name, value);
    }
}

} // namespace analyser
