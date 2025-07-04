#pragma once

#include <string>

#include "metric.hpp"

namespace analyser::metric::metric_impl {

struct CodeLinesCountMetric final : IMetric {
    ~CodeLinesCountMetric() override = default;

protected:
    std::string Name() const override { return "code_lines"; }

    MetricResult::ValueType CalculateImpl(const function::Function &f) const override;
};

} // namespace analyser::metric::metric_impl
