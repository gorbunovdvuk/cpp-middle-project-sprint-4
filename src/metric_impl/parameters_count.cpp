#include "metric_impl/parameters_count.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <range/v3/all.hpp>
#include <ranges>
#include <sstream>
#include <string>

namespace analyser::metric::metric_impl {

MetricResult::ValueType CountParametersMetric::CalculateImpl(const function::Function &f) const {
    const std::string& ast = f.ast;
    if (std::size_t start = ast.find("(parameters"); start == std::string::npos) {
        return MetricResult::ValueType{0};
    } else {
        static constexpr std::array<std::string_view, 6> kPossibleParameters = {
            "identifier",
            "default_parameter",
            "typed_parameter",
            "typed_default_parameter",
            "list_splat_pattern",
            "dictionary_splat_pattern"
        };

        std::string_view current_ast = std::string_view(ast).substr(start);

        auto indexed_balance = current_ast |
            ranges::views::transform([](char c) { return c == '(' ? 1 : (c == ')' ? -1 : 0); }) |
            ranges::views::partial_sum |
            ranges::views::take_while([](int i) { return i > 0; }) |
            ranges::views::enumerate;

        auto is_parameter = [&current_ast](const auto& prev, const auto& cur) {
            const auto [pos1, balance1] = prev;
            const auto [pos2, balance2] = cur;
            if (balance1 != 1 || balance2 != 2) {
                return false;
            }
            return std::ranges::any_of(kPossibleParameters, [&current_ast, pos2](std::string_view match) {
                return current_ast.substr(pos2 + 1).starts_with(match);
            });
        };
        return static_cast<MetricResult::ValueType>(ranges::distance(
            indexed_balance | ranges::views::adjacent_filter(is_parameter)
        ) - 1);
    }
}

}  // namespace analyser::metric::metric_impl
