#include "../../utils/file_utils.h"
#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>
#include <cmath>
#include <unordered_set>
#include <chrono>

const char* DATA_FILEPATH = "../data/data";
const char* EXAMPLE_FILEPATH = "../data/example";

int digits(long long n) {
    if (n == 0) return 1;
    return static_cast<int>(std::floor(std::log10(std::abs(n)))) + 1;
}

struct Range {
    long long start;
    long long end;

    Range(std::string s) {
        // Two integers separated by a -
        int dash_pos = s.find('-');
        start = std::stoll(s.substr(0, dash_pos));
        end = std::stoll(s.substr(dash_pos + 1));
        // std::cout << "Parsed Range: " << start << " to " << end << std::endl;
    }
};

std::vector<Range> parse(const std::string& content) {
    std::vector<Range> instructions;
    std::istringstream stream(content);
    std::string token;
    while (std::getline(stream, token, ',')) {
        instructions.emplace_back(token);
    }
    return instructions;
}

bool is_in_range(const std::vector<Range>& ranges, long long value) {
    for (const auto& range : ranges) {
        if (value >= range.start && value <= range.end) {
            return true;
        }
    }
    return false;
}

long long process1(const std::vector<Range>& instructions) {
    const auto begin = std::chrono::high_resolution_clock::now();
    const int repetition = 2;
    // Get longest range (highest end)
    long long max_digits = 0;
    for (const auto& instr : instructions) {
        int start_digits = digits(instr.start);
        int end_digits = digits(instr.end);
        if (end_digits > max_digits) {
            max_digits = end_digits;
        }
        if (start_digits > max_digits) {
            max_digits = start_digits;
        }
    }
    // std::cout << "Longest is " << max_digits << " digits" << std::endl;
    int tested_value = 1;
    long long result = 0;
    while(true) {
        std::string concatenated_value = "";
        for (int rep = 0; rep < repetition; rep++) {
            concatenated_value += std::to_string(tested_value);
        }
        long long numeric_value = std::stoll(concatenated_value);
        if (is_in_range(instructions, numeric_value)) {
            result += numeric_value;
            // std::cout << numeric_value << " matched! Current sum: " << result << std::endl;
        }
        tested_value++;
        if (digits(tested_value) > max_digits / 2) {
            break;
        } 
    }
    const auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Processing time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " µs" << std::endl;
    return result;
}

long long process2(const std::vector<Range>& instructions) {
    const auto begin = std::chrono::high_resolution_clock::now();
    // Get longest range (highest end)
    long long max_digits = 0;
    for (const auto& instr : instructions) {
        int start_digits = digits(instr.start);
        int end_digits = digits(instr.end);
        if (end_digits > max_digits) {
            max_digits = end_digits;
        }
        if (start_digits > max_digits) {
            max_digits = start_digits;
        }
    }
    int tested_value = 1;
    long long result = 0;
    std::unordered_set<long long> encountered_values;
    while(true) {
        const int tested_value_digits = digits(tested_value);
        std::string concatenated_value = std::to_string(tested_value);
        while (true) {
            concatenated_value += std::to_string(tested_value);
            if (concatenated_value.length() > max_digits) {
                break;
            }
            long long numeric_value = std::stoll(concatenated_value);
            if (encountered_values.find(numeric_value) == encountered_values.end() && is_in_range(instructions, numeric_value)) {
                result += numeric_value;
                encountered_values.insert(numeric_value);
            }
        }
        tested_value++;
        if (digits(tested_value) > max_digits / 2) {
            break;
        } 
    }
    const auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Processing time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " µs" << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms*" << std::endl;
    return result;
}

bool run_example1() {
    const auto data_content = read_file(EXAMPLE_FILEPATH);
    auto instructions = parse(data_content);
    long long result = process1(instructions);
    return result == 1227775554;
}

bool run_example2() {
    const auto data_content = read_file(EXAMPLE_FILEPATH);
    auto instructions = parse(data_content);
    long long result = process2(instructions);
    return result == 4174379265;
}

bool run_data1() {
    const auto data_content = read_file(DATA_FILEPATH);
    auto instructions = parse(data_content);
    long long result = process1(instructions);
    std::cout << "Final result: " << result << std::endl;
    return true;
}

bool run_data2() {
    const auto data_content = read_file(DATA_FILEPATH);
    auto instructions = parse(data_content);
    long long result = process2(instructions);
    std::cout << "Final result: " << result << std::endl;
    return true;
}

int main() {
    // assert(run_example1());
    // run_data1();
    // assert(run_example2());
    run_data2();
    return 0;
}