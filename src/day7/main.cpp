#include "file_utils.h"
#include "math_utils.h"
#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>

const char* DATA_FILEPATH = "./data/day7/data";
const char* EXAMPLE_FILEPATH = "./data/day7/example";
const char* TEST_FILEPATH = "./data/day7/test";

// std::vector<MathSheet> parse(const std::string& content) {
//     std::vector<MathSheet> sheets;
//     std::istringstream stream(content);
//     std::string line;
//     while (std::getline(stream, line)) {
//         if (line.empty()) continue;
//         // Split line into tokens separated by spaces of any length
//         std::istringstream line_stream(line);
//         std::string token;
//         int token_index = 0;
//         while (line_stream >> token) {
//             if (sheets.size() <= token_index) {
//                 sheets.emplace_back();
//             }
//             MathSheet& sheet = sheets[token_index];
//             if (token == "+") {
//                 sheet.operation = MathSheet::ADD;
//             } else if (token == "-") {
//                 sheet.operation = MathSheet::SUBTRACT;
//             } else if (token == "*") {
//                 sheet.operation = MathSheet::MULTIPLY;
//             } else if (token == "/") {
//                 sheet.operation = MathSheet::DIVIDE;
//             } else {
//                 sheet.numbers.push_back(std::stoll(token));
//             }
//             token_index++;
//         }
//     }
//     std::cout << "Parsed " << sheets.size() << " sheets." << std::endl;
//     return sheets;
// }

// long long process1(const std::vector<MathSheet>& sheets) {
//     const auto begin = std::chrono::high_resolution_clock::now();
//     long long result = 0;
//     for (const auto& sheet : sheets) {
//         result += sheet.compute();
//     }
//     const auto end = std::chrono::high_resolution_clock::now();
//     const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
//     std::cout << "Processing time: " << duration << " us" << std::endl;
//     return result;
// }

// bool run_example1() {
//     const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
//     const auto sheets = parse(data_content);
//     long long result = process1(sheets);
//     std::cout << "Example1 result: " << result << std::endl;
//     return result == 4277556;
// }

// bool run_data1() {
//     const auto data_content = utils::read_file(DATA_FILEPATH);
//     const auto sheets = parse(data_content);
//     long long result = process1(sheets);
//     std::cout << "Data1 result: " << result << std::endl;
//     return true;
// }

// bool run_example2() {
//     const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
//     auto sheets = parse2(data_content);
//     long long result = process1(sheets);
//     std::cout << "Example2 result: " << result << std::endl;
//     return result == 3263827;
// }

// bool run_data2() {
//     const auto data_content = utils::read_file(DATA_FILEPATH);
//     const auto sheets = parse2(data_content);
//     long long result = process1(sheets);
//     std::cout << "Data2 result: " << result << std::endl;
//     return true;
// }

int main() {
    // if (!run_example1()) {
    //     std::cerr << "Example1 failed!" << std::endl;
    //     return 1;
    // }
    // std::cout << "Example1 passed!" << std::endl;
    // run_data1();
    // if(!run_example2()) {
    //     std::cerr << "Example2 failed!" << std::endl;
    //     return 1;
    // }
    // std::cout << "Example2 passed!" << std::endl;
    // run_data2();
    return 0;
}