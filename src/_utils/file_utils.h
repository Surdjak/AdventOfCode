#pragma once
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <vector>
#include <string>

namespace utils {
    std::string read_file(const std::filesystem::path& filepath);
    std::vector<std::string> split_string(const std::string& str, char delimiter);
}
