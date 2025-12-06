#include "file_utils.h"
#include "math_utils.h"
#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>

const char* DATA_FILEPATH = "./data/day6/data";
const char* EXAMPLE_FILEPATH = "./data/day6/example";
const char* TEST_FILEPATH = "./data/day6/test";

struct MathSheet {
    enum Symbol {
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE
    };

    std::vector<long long> numbers;
    Symbol operation;

    long long compute() const {
        long long result = 0;
        if (numbers.empty()) return result;
        result = numbers[0];
        for (size_t i = 1; i < numbers.size(); ++i) {
            switch (operation) {
                case ADD:
                    result += numbers[i];
                    break;
                case SUBTRACT:
                    result -= numbers[i];
                    break;
                case MULTIPLY:
                    result *= numbers[i];
                    break;
                case DIVIDE:
                    if (numbers[i] != 0) {
                        result /= numbers[i];
                    } else {
                        std::cerr << "Warning: Division by zero encountered." << std::endl;
                    }
                    break;
            }
        }
        return result;
    }

    void reverse_numbers() {
        std::reverse(numbers.begin(), numbers.end());
    }
};

std::vector<MathSheet> parse(const std::string& content) {
    std::vector<MathSheet> sheets;
    std::istringstream stream(content);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        // Split line into tokens separated by spaces of any length
        std::istringstream line_stream(line);
        std::string token;
        int token_index = 0;
        while (line_stream >> token) {
            if (sheets.size() <= token_index) {
                sheets.emplace_back();
            }
            MathSheet& sheet = sheets[token_index];
            if (token == "+") {
                sheet.operation = MathSheet::ADD;
            } else if (token == "-") {
                sheet.operation = MathSheet::SUBTRACT;
            } else if (token == "*") {
                sheet.operation = MathSheet::MULTIPLY;
            } else if (token == "/") {
                sheet.operation = MathSheet::DIVIDE;
            } else {
                sheet.numbers.push_back(std::stoll(token));
            }
            token_index++;
        }
    }
    std::cout << "Parsed " << sheets.size() << " sheets." << std::endl;
    return sheets;
}

std::vector<MathSheet> parse2(const std::string& content) {
    std::vector<MathSheet> sheets;
    std::istringstream stream(content);
    std::string line;
    std::vector<std::vector<char>> raw_sheets;
    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        std::vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        raw_sheets.push_back(row);
    }
    for (int column = 0; column < raw_sheets[0].size(); ++column) {
        const char operator_char = raw_sheets[raw_sheets.size() - 1][column];
        if (operator_char != ' ') {
            if (sheets.size() > 0) {
                sheets.back().reverse_numbers();
                // std::cout << "Completed sheet with values:" << std::endl;
                // for (const auto& num : sheets.back().numbers) {
                //     std::cout << num << std::endl;
                // }
            }
            sheets.emplace_back();
            // std::cout << "New sheet with operator " << operator_char << std::endl;
            switch (operator_char) {
                case '+':
                    sheets.back().operation = MathSheet::ADD;
                    break;
                case '-':
                    sheets.back().operation = MathSheet::SUBTRACT;
                    break;
                case '*':
                    sheets.back().operation = MathSheet::MULTIPLY;
                    break;
                case '/':
                    sheets.back().operation = MathSheet::DIVIDE;
                    break;
                default:
                    throw std::runtime_error("Unknown operator character in parse2");
            }
        }
        long long value = 0;
        bool found_digit = false;
        for (int row = 0; row < raw_sheets.size() - 1; ++row) {
            const char c = raw_sheets[row][column];
            if (c >= '0' && c <= '9') {
                found_digit = true;
                value = value * 10 + (c - '0');
            } else if (value != 0) {
                break;
            }
        }
        if (!found_digit) {
            continue;
        }
        sheets.back().numbers.push_back(value);
        // std::cout << "Added number " << value << " to current sheet." << std::endl;
    }
    if (sheets.size() > 0) {
        sheets.back().reverse_numbers();
        // std::cout << "Completed sheet with values:" << std::endl;
        // for (const auto& num : sheets.back().numbers) {
        //     std::cout << num << std::endl;
        // }
    }
    std::cout << "Parsed " << sheets.size() << " sheets." << std::endl;
    return sheets;
}

long long process1(const std::vector<MathSheet>& sheets) {
    const auto begin = std::chrono::high_resolution_clock::now();
    long long result = 0;
    for (const auto& sheet : sheets) {
        result += sheet.compute();
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Processing time: " << duration << " us" << std::endl;
    return result;
}

bool run_example1() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    const auto sheets = parse(data_content);
    long long result = process1(sheets);
    std::cout << "Example1 result: " << result << std::endl;
    return result == 4277556;
}

bool run_data1() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    const auto sheets = parse(data_content);
    long long result = process1(sheets);
    std::cout << "Data1 result: " << result << std::endl;
    return true;
}

bool run_example2() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    auto sheets = parse2(data_content);
    long long result = process1(sheets);
    std::cout << "Example2 result: " << result << std::endl;
    return result == 3263827;
}

bool run_data2() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    const auto sheets = parse2(data_content);
    long long result = process1(sheets);
    std::cout << "Data2 result: " << result << std::endl;
    return true;
}

// bool run_test() {
//     const auto data_content = utils::read_file(TEST_FILEPATH);
//     const auto ingredients = parse(data_content);
//     long long result = process2(ingredients);
//     std::cout << "Test result: " << result << std::endl;
//     return result == 12;
// }

int main() {
    // if (!run_example1()) {
    //     std::cerr << "Example1 failed!" << std::endl;
    //     return 1;
    // }
    // std::cout << "Example1 passed!" << std::endl;
    // run_data1();
    if(!run_example2()) {
        std::cerr << "Example2 failed!" << std::endl;
        return 1;
    }
    std::cout << "Example2 passed!" << std::endl;
    run_data2();
    return 0;
}