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

    int get_largest_joltage_for_2_batteries() const {
        int largest_joltage = 0;
        for(int i = 0; i < batteries.size(); ++i) {
            for(int j = i + 1; j < batteries.size(); ++j) {
                int joltage = batteries[i] * 10 + batteries[j];
                if (joltage > largest_joltage) {
                    largest_joltage = joltage;
                }
            }
        }
        return largest_joltage;
    }

    int get_largest_joltage_for_x_batteries(const int x) const {
        int largest_joltage = 0;
        std::vector<int> indices(x);
        return largest_joltage;
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

int process1(const std::vector<Bank>& banks) {
    int total = 0;
    for (const auto& bank : banks) {
        int to_add = bank.get_largest_joltage();
        std::cout << "Bank largest joltage: " << to_add << std::endl;
        total += to_add;
    }
    return total;
}

bool run_example1() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    auto banks = parse(data_content);
    long long result = process1(banks);
    std::cout << "Example1 result: " << result << std::endl;
    return result == 357;
}

bool run_data1() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    auto banks = parse(data_content);
    long long result = process1(banks);
    std::cout << "Data1 result: " << result << std::endl;
    return true;
}

int main() {
    run_example1();
    run_data1();
    // assert(run_example2());
    return 0;
}