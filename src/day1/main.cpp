#include "dial.h"
#include "file_utils.h"
#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>

using namespace day1;

const char* DATA_FILEPATH = "./data/day1/data";
const char* EXAMPLE_FILEPATH = "./data/day1/example";

std::vector<Dial::Instruction> parse_instructions(const std::string& content) {
    std::vector<Dial::Instruction> instructions;
    std::istringstream stream(content);
    std::string line;
    while (std::getline(stream, line)) {
        if (!line.empty()) {
            instructions.emplace_back(line);
        }
    }
    return instructions;
}

template<bool JUMP>
int process(std::vector<Dial::Instruction> instructions) {
    const auto begin = std::chrono::high_resolution_clock::now();
    Dial dial(100);
    for (const auto& instr : instructions) {
        if constexpr (JUMP) {
            dial.jump(instr);
        } else {
            dial.turn(instr);
        }
    }
    const auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Processing time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " µs" << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms*" << std::endl;
    return dial.getEncounterCount(0);
}

bool run_example1() {
    Dial dial(100);
    const auto example_content = utils::read_file(EXAMPLE_FILEPATH);
    auto instructions = parse_instructions(example_content);
    const auto result = process<true>(instructions);
    return result == 3;
}

bool run_example2() {
    Dial dial(100);
    const auto example_content = utils::read_file(EXAMPLE_FILEPATH);
    auto instructions = parse_instructions(example_content);
    const auto result = process<false>(instructions);
    return result == 6;
}

bool run_data1() {
    Dial dial(100);
    const auto data_content = utils::read_file(DATA_FILEPATH);
    auto instructions = parse_instructions(data_content);
    const auto result = process<true>(instructions);
    std::cout << "Data1 Result: " << result << std::endl;
    return true;
}

bool run_data2() {
    Dial dial(100);
    const auto data_content = utils::read_file(DATA_FILEPATH);
    auto instructions = parse_instructions(data_content);
    const auto result = process<false>(instructions);
    std::cout << "Data2 Result: " << result << std::endl;
    return true;
}

int main() {
    assert(run_example1());
    run_data1();
    assert(run_example2());
    run_data2();
    return 0;
}