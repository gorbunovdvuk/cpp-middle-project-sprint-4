#include "metric_impl/code_lines_count.hpp"

#include <iostream>
#include <ranges>
#include <regex>
#include <string>
#include <unordered_set>

namespace analyser::metric::metric_impl {

MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function &f) const {
    static const std::regex regex{R"(\((\w+) \[(\d+),)"};
    return static_cast<MetricResult::ValueType>(std::unordered_set<int>(std::from_range, std::ranges::subrange(
        std::sregex_iterator(f.ast.begin(), f.ast.end(), regex),
        std::sregex_iterator()
    ) | std::views::filter([](const std::smatch& match) {
        return match[1].str() != "comment";
    }) | std::views::transform([](const std::smatch& match) {
        return std::stoi(match[2].str());
    })).size());
}

// здесь ваш код

}  // namespace analyser::metric::metric_impl
