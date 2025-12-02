#include "day1/dial.h"
#include "utils/file_utils.h"
#include <sstream>
#include <vector>
#include <cassert>

using namespace day1;

const char* DATA_FILEPATH = "../data/data";
const char* EXAMPLE_FILEPATH = "../data/example";

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

bool run_example1() {
    Dial dial(100);
    const auto example_content = utils::read_file(EXAMPLE_FILEPATH);
    auto instructions = parse_instructions(example_content);
    for (const auto& instr : instructions) {
        dial.jump(instr);
    }
    return dial.getEncounterCount(0) == 3;
}

bool run_example2() {
    Dial dial(100);
    const auto example_content = utils::read_file(EXAMPLE_FILEPATH);
    auto instructions = parse_instructions(example_content);
    for (const auto& instr : instructions) {
        dial.turn(instr);
    }
    return dial.getEncounterCount(0) == 6;
}

bool run_data1() {
    Dial dial(100);
    const auto data_content = utils::read_file(DATA_FILEPATH);
    auto instructions = parse_instructions(data_content);
    for (const auto& instr : instructions) {
        dial.jump(instr);
    }
    dial.log_encountered_state(0);
    return true;
}

bool run_data2() {
    Dial dial(100);
    const auto data_content = utils::read_file(DATA_FILEPATH);
    auto instructions = parse_instructions(data_content);
    for (const auto& instr : instructions) {
        dial.turn(instr);
    }
    dial.log_encountered_state(0);
    return true;
}

int main() {
    assert(run_example1());
    run_data1();
    assert(run_example2());
    run_data2();
    return 0;
}