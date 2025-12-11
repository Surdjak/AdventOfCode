#pragma once
#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace utils {
    std::string read_file(const std::filesystem::path& filepath);
}
