#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
#include <print>
#include <ranges>
#include <string>
#include <vector>

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

namespace accumulators = metric_accumulator_impl;
namespace metrics = metric_impl;

void PrintAccumulatorResults(MetricsAccumulator& accumulator, size_t indent = 4) {
    const auto code_lines = accumulator.GetFinalizedAccumulator<accumulators::SumAverageAccumulator>("code_lines");
    const auto parameters = accumulator.GetFinalizedAccumulator<accumulators::AverageAccumulator>("parameters");
    const auto complexity = accumulator.GetFinalizedAccumulator<accumulators::SumAverageAccumulator>("complexity");

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

int main(int argc, char *argv[]) {
    try {
        analyser::cmd::ProgramOptions options;

        if (!options.Parse(argc, argv)) {
            return 1;
        }

        MetricExtractor metric_extractor;
        metric_extractor.RegisterMetric(std::make_unique<metrics::CodeLinesCountMetric>());
        metric_extractor.RegisterMetric(std::make_unique<metrics::CountParametersMetric>());
        metric_extractor.RegisterMetric(std::make_unique<metrics::CyclomaticComplexityMetric>());

        MetricsAccumulator accumulator;
        accumulator.RegisterAccumulator("code_lines", std::make_unique<accumulators::SumAverageAccumulator>());
        accumulator.RegisterAccumulator("parameters", std::make_unique<accumulators::AverageAccumulator>());
        accumulator.RegisterAccumulator("complexity", std::make_unique<accumulators::SumAverageAccumulator>());

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
            PrintAccumulatorResults(accumulator);
        }

        const auto by_classes = analyser::SplitByClasses(analysis);

        for (const auto& [class_name, class_analysis] : by_classes) {
            std::print("Accumulated Analysis for class {}\n", class_name);
            analyser::AccumulateFunctionAnalysis(class_analysis, accumulator);
            PrintAccumulatorResults(accumulator);
        }

        std::print("Accumulated Analysis for all functions\n");
        analyser::AccumulateFunctionAnalysis(analysis, accumulator);
        PrintAccumulatorResults(accumulator);
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
