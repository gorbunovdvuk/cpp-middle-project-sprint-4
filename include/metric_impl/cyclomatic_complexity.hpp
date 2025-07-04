#pragma once

#include <string>

#include "metric.hpp"

namespace analyser::metric::metric_impl {

struct CyclomaticComplexityMetric final : IMetric {
    ~CyclomaticComplexityMetric() override = default;

protected:
    std::string Name() const override { return "complexity"; }

    MetricResult::ValueType CalculateImpl(const function::Function &f) const override;
};

} // namespace analyser::metric::metric_impl
