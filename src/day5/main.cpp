#include "file_utils.h"
#include "math_utils.h"
#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>

const char* DATA_FILEPATH = "./data/day5/data";
const char* EXAMPLE_FILEPATH = "./data/day5/example";
const char* TEST_FILEPATH = "./data/day5/test";

struct Ingredients {
    std::vector<std::pair<long long, long long>> ranges;
    std::vector<long long> ids;

    void merge_overlapping_ranges() {
        if (ranges.empty()) return;
        std::sort(ranges.begin(), ranges.end());
        std::vector<std::pair<long long, long long>> merged;
        merged.push_back(ranges[0]);
        for (size_t i = 1; i < ranges.size(); ++i) {
            if (ranges[i].first <= merged.back().second) {
                merged.back().second = std::max(merged.back().second, ranges[i].second);
            } else {
                merged.push_back(ranges[i]);
            }
        }
        ranges = std::move(merged);
    }
};

Ingredients parse(const std::string& content) {
    Ingredients ingredients;
    std::istringstream stream(content);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        const int dash_pos = line.find('-');
        if (dash_pos != std::string::npos) {
            const long long first = std::stoll(line.substr(0, dash_pos));
            const long long second = std::stoll(line.substr(dash_pos + 1));
            ingredients.ranges.emplace_back(first, second);
        } else {
            // Otherwise, it's an ID line
            ingredients.ids.push_back(std::stoll(line));
        }
    }
    ingredients.merge_overlapping_ranges();
    return ingredients;
}

int process1(const Ingredients& ingredients) {
    const auto begin = std::chrono::high_resolution_clock::now();
    long long fresh = 0;
    for (long long id : ingredients.ids) {
        for (const auto& range : ingredients.ranges) {
            if (id >= range.first && id <= range.second) {
                fresh++;
                break;
            }
        }
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Processing time: " << duration << " us" << std::endl;
    return fresh;
}

long long process2(const Ingredients& ingredients) {
    const auto begin = std::chrono::high_resolution_clock::now();
    long long fresh_range = 0;
    for(const auto range : ingredients.ranges) {
        fresh_range += (range.second - range.first + long long(1));
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Processing time: " << duration << " us" << std::endl;
    return fresh_range;
}

bool run_example1() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    const auto ingredients = parse(data_content);
    long long result = process1(ingredients);
    std::cout << "Example1 result: " << result << std::endl;
    return result == 3;
}

bool run_data1() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    const auto ingredients = parse(data_content);
    long long result = process1(ingredients);
    std::cout << "Data1 result: " << result << std::endl;
    return true;
}

bool run_example2() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    const auto ingredients = parse(data_content);
    long long result = process2(ingredients);
    std::cout << "Example2 result: " << result << std::endl;
    return result == 14;
}

bool run_data2() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    const auto ingredients = parse(data_content);
    long long result = process2(ingredients);
    std::cout << "Data2 result: " << result << std::endl;
    return true;
}

bool run_test() {
    const auto data_content = utils::read_file(TEST_FILEPATH);
    const auto ingredients = parse(data_content);
    long long result = process2(ingredients);
    std::cout << "Test result: " << result << std::endl;
    return result == 12;
}

int main() {
    if (!run_example1()) {
        std::cerr << "Example1 failed!" << std::endl;
        return 1;
    }
    std::cout << "Example1 passed!" << std::endl;
    run_data1();
    if(!run_example2()) {
        std::cerr << "Example2 failed!" << std::endl;
        return 1;
    }
    std::cout << "Example2 passed!" << std::endl;
    run_data2();
    return 0;
}