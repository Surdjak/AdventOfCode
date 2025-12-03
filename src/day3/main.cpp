#include "utils/file_utils.h"
#include "utils/math_utils.h"
#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>

const char* DATA_FILEPATH = "./data/day3/data";
const char* EXAMPLE_FILEPATH = "./data/day3/example";

class Bank {
    std::vector<int> batteries;
public:
    Bank(const std::string& content) {
        for (char c : content) {
            if (std::isdigit(c)) {
                batteries.push_back(c - '0');
            }
        }
    }

    long long get_largest_joltage(const int x) const {
        long long total_joltage = 0;
        int start = 0;
        for (int i = 0; i < x; ++i) {
            for (int l = 9; l >= 0; --l) {
                bool found = false;
                for(int b = start; b < batteries.size() - (x - i - 1); ++b) {
                    if (batteries[b] == l) {
                        total_joltage = total_joltage * 10 + batteries[b];
                        start = b + 1;
                        found = true;
                        break;
                    }
                }
                if (found) {
                    break;
                }
            }       
        }
        return total_joltage;
    }
};

std::vector<Bank> parse(const std::string& content) {
    std::vector<Bank> banks;
    std::istringstream stream(content);
    std::string line;
    while (std::getline(stream, line)) {
        banks.emplace_back(line);
    }
    return banks;
}

long long process(const std::vector<Bank>& banks, const int x) {
    const auto begin = std::chrono::high_resolution_clock::now();
    long long total = 0;
    for (const auto& bank : banks) {
        long long to_add = bank.get_largest_joltage(x);
        total += to_add;
    }
    const auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Processing time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " µs" << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms*" << std::endl;
    return total;
}

bool run_example1() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    auto banks = parse(data_content);
    long long result = process(banks, 2);
    std::cout << "Example1 result: " << result << std::endl;
    return result == 357;
}

bool run_data1() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    auto banks = parse(data_content);
    long long result = process(banks, 2);
    std::cout << "Data1 result: " << result << std::endl;
    return true;
}

bool run_example2() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    auto banks = parse(data_content);
    long long result = process(banks, 12);
    std::cout << "Example2 result: " << result << std::endl;
    return result == 3121910778619;
}

bool run_data2() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    auto banks = parse(data_content);
    long long result = process(banks, 12);
    std::cout << "Data2 result: " << result << std::endl;
    return true;
}

int main() {
    run_example1();
    run_data1();
    run_example2();
    run_data2();
    return 0;
}