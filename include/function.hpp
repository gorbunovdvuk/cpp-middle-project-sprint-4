#pragma once

#include <filesystem>
#include <ranges>
#include <string>
#include <vector>

#include "file.hpp"

namespace fs = std::filesystem;
namespace rv = std::ranges::views;
namespace rs = std::ranges;

namespace analyser::function {

struct Function {
    std::string filename;
    std::optional<std::string> class_name;
    std::string name;
    std::string ast;
};

struct FunctionExtractor {
    static std::vector<Function> Get(const file::File& file);

private:
    struct Position {
        size_t line;
        size_t col;
    };

    struct FunctionNameLocation {
        Position start;
        Position end;
        std::string name;
    };

    struct ClassInfo {
        std::string name;
        Position start;
        Position end;
    };

    static FunctionNameLocation GetNameLocation(const std::string& function_ast);
    static std::string GetNameFromSource(const std::string& function_ast,
                                  const std::vector<std::string>& lines);
    static std::optional<ClassInfo> FindEnclosingClass(const std::string& ast,
                                                const FunctionNameLocation& func_loc);
    static std::string GetClassNameFromSource(const ClassInfo& class_info,
                                       const std::vector<std::string>& lines);
};

} // namespace analyser::function
