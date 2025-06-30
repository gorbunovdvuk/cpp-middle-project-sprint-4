#include <string>

#include "metric.hpp"

namespace analyser::metric::metric_impl {

struct CountParametersMetric final : IMetric {
    ~CountParametersMetric() override = default;

protected:
    std::string Name() const override { return "parameters"; }

    MetricResult::ValueType CalculateImpl(const function::Function &f) const override;
};

} // namespace analyser::metric::metric_impl
