#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
#include <format>

#include "analyse.hpp"
#include "cmd_options.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
#include "metric_accumulator_impl/accumulators.hpp"
#include "metric_impl/metrics.hpp"

using namespace analyser::function;
using namespace analyser::file;
using namespace analyser::metric;
using namespace analyser::metric_accumulator;

int main(int argc, char *argv[]) {
    analyser::cmd::ProgramOptions options;

    if (!options.Parse(argc, argv)) {
        return 1;
    }

    MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CodeLinesCountMetric>());
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CountParametersMetric>());
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CyclomaticComplexityMetric>());

    MetricsAccumulator accumulator;
    accumulator.RegisterAccumulator("code_lines", std::make_unique<metric_accumulator_impl::SumAverageAccumulator>());
    accumulator.RegisterAccumulator("parameters", std::make_unique<metric_accumulator_impl::AverageAccumulator>());
    accumulator.RegisterAccumulator("complexity", std::make_unique<metric_accumulator_impl::SumAverageAccumulator>());

    auto analysis = analyser::AnalyseFunctions(options.GetFiles(), metric_extractor);

    for (const auto& [function, metrics] : analysis) {
        std::print("{}{}{}::{}():\n", function.filename, function.class_name.has_value() ? "::" : "", function.class_name.value_or(""), function.name);
        std::ranges::for_each(metrics, [](const auto& metric) {
            std::print("    {}: {}\n", metric.metric_name, metric.value);
        });
    }

    const auto by_files = analyser::SplitByFiles(analysis);

    for (const auto& [file_name, file_analysis] : by_files) {
        std::print("Accumulated Analysis for file {}\n", file_name);
        analyser::AccumulateFunctionAnalysis(file_analysis, accumulator);
    }

    const auto by_classes = analyser::SplitByClasses(analysis);

    for (const auto& [class_name, class_analysis] : by_classes) {
        std::print("Accumulated Analysis for class {}\n", class_name);
        analyser::AccumulateFunctionAnalysis(class_analysis, accumulator);
    }

    std::print("Accumulated Analysis for all functions\n");
    analyser::AccumulateFunctionAnalysis(analysis, accumulator);

    return 0;
}
