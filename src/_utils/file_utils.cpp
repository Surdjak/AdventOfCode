#include <fstream>
#include <stdexcept>
#include <filesystem>
#include "file_utils.h"

std::string utils::read_file(const std::filesystem::path& filepath) {
    try {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filepath.string());
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        return content;
    } catch (const std::exception& e) {
        throw std::runtime_error("Error reading file " + filepath.string() + ": " + e.what());
    }
}
