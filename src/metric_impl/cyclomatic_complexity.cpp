#include "metric_impl/cyclomatic_complexity.hpp"

#include <array>
#include <ranges>
#include <sstream>
#include <string>

#include <boost/algorithm/string.hpp>

namespace analyser::metric::metric_impl {

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f) const {
    static constexpr std::array<std::string_view, 12> kElements = {
        "if_statement",
        "elif_clause",
        "else_clause",
        "while_statement",
        "for_statement",
        "try_statement",
        "except_clause",
        "finally_clause",
        "match_statement",
        "case_clause",
        "assert_statement",
        "conditional_expression"
    };
    const std::string& ast = f.ast;
    return static_cast<MetricResult::ValueType>(
        std::ranges::distance(kElements |
            std::views::transform([&ast](std::string_view element) {
                return std::ranges::subrange(
                    boost::make_find_iterator(ast, boost::first_finder(element)),
                    boost::find_iterator<std::string::const_iterator>{}
                );
            }
        ) | std::views::join) + 1);
}

}  // namespace analyser::metric::metric_impl
