#pragma once

#include <fstream>
#include <functional>
#include <cstring>
#include <iostream>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

namespace analyser::file {

struct File {
    static inline const std::string command_prefix = std::invoke([]() {
        std::string result = "tree-sitter parse ";
        if (auto value = std::getenv("USE_HOME_CONFIG"); value == nullptr || std::string_view(value) != "1") {
            result += "--config-path /root/.config/tree-sitter/config.json ";
        }
        return result;
    });

    File(const std::string& filename);
    std::string name;
    std::string ast;
    std::vector<std::string> source_lines;

private:
    std::vector<std::string> ReadSourceFile(std::ifstream& file);
    std::string GetAst(const std::string& filename);
};

} // namespace analyser::file
