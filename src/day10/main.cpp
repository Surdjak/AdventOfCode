#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>
#include <array>
#include <unordered_set>
#include <queue>
#include <map>
#include <numeric>
#include <ranges>
#include "file_utils.h"
#include "math_utils.h"
#include "debug_utils.h"
#include "logic_utils.h"

const char* DATA_FILEPATH = "./data/day10/data";
const char* EXAMPLE_FILEPATH = "./data/day10/example";
const char* TEST_FILEPATH = "./data/day10/test";

struct Machine {
    std::vector<bool> target_state;
    std::vector<std::vector<int>> buttons;
    std::vector<int> joltage_requirement;
};

std::vector<Machine> parse(const std::string& content) {
    std::vector<Machine> machines;
    std::istringstream ss(content);
    std::string line;
    while(std::getline(ss, line)) {
        Machine machine;
        // First get the first group between the []
        auto parts = utils::split_string(line, ' ');
        for (const auto& part : parts) {
            // std::cout << part << std::endl;
            if (part[0] == '[') {
                // init state
                // strip the []
                const auto stripped = part.substr(1, part.size() - 2);
                for (const auto& elem : stripped) {
                    // std::cout << elem << std::endl;
                    if (elem == '#') {
                        machine.target_state.push_back(true);
                    } else {
                        machine.target_state.push_back(false);
                    }
                }
            } else if (part[0] == '(') {
                // button
                // strip the ()
                const auto stripped = part.substr(1, part.size() - 2);
                const auto elements = utils::split_string(stripped, ',');
                std::vector<int> button;;
                for (const auto& elem : elements) {
                    // std::cout << elem << std::endl;
                    button.push_back(std::stoi(elem));
                }
                machine.buttons.push_back(button);
            } else {
                // joltage requirement
                // strip the {}
                const auto stripped = part.substr(1, part.size() - 2);
                const auto elements = utils::split_string(stripped, ',');
                for (const auto& elem : elements) {
                    // std::cout << elem << std::endl;
                    machine.joltage_requirement.push_back(std::stoi(elem));
                }
            }
        }
        // std::cout << std::format("Parsed machine with {} lights, {} buttons, {} joltage requirements values\n",
            // machine.target_state.size(), machine.buttons.size(), machine.joltage_requirement.size());
        machines.push_back(machine);
    }
    return machines;
}

long long process1(std::vector<Machine>& machines) {
    const auto begin = std::chrono::high_resolution_clock::now();
    long long result = 0;
    for(const auto& machine : machines) {
        std::vector<std::vector<bool>> states = {std::vector<bool>(machine.target_state.size(), false)};
        if(states[0] == machine.target_state) {
            continue;
        }
        int current_button_presses = 1;
        while(true) {
            bool found = false;
            std::vector<std::vector<bool>> next_states;
            next_states.reserve(states.size() * machine.buttons.size());
            for(const auto& state : states) {
                for(const auto& button : machine.buttons) {
                    auto new_state = state;
                    for (const auto& light_index : button) {
                        new_state[light_index] = !new_state[light_index];
                    }
                    if(new_state == machine.target_state) {
                        found = true;
                        break;
                    }
                    next_states.push_back(new_state);
                }
                if(found) {
                    break;
                }
            }
            if(found) {
                std::cout << "Found solution with " << current_button_presses << " button presses." << std::endl;
                result+=current_button_presses;
                break;
            }
            states = std::move(next_states);
            current_button_presses++;
        }
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Processing time: " << duration << " us" << std::endl;
    return result;
}

class ButtonPresser {
    std::vector<std::vector<int>>& buttons;
    std::vector<int>& target_state;
    long long best = std::numeric_limits<long long>::max();
public:
    ButtonPresser(const std::vector<std::vector<int>>& buttons_, const std::vector<int>& target_state_)
        : buttons(const_cast<std::vector<std::vector<int>>&>(buttons_)),
          target_state(const_cast<std::vector<int>&>(target_state_)) {
        press_button_rec(std::vector<int>(target_state.size(), 0), 0);
    }

    void press_button_rec(const std::vector<int>& current_state, long long depth) {
        // std::cout << std::format("Depth is {}\n", depth);
        if(depth >= best) {
            return;
        }
        if(current_state == target_state) {
            best = depth;
            std::cout << std::format("New best solution with {} button presses.\n", best);
            return;
        }
        for(const auto& button : buttons) {
            bool overflow = false;
            std::vector<int> new_state = current_state;
            for(const auto& index : button) {
                new_state[index] += 1;
                if(new_state[index] > target_state[index]) {
                    overflow = true;
                    break;
                }
            }
            if(!overflow) {
                press_button_rec(new_state, depth + 1);
            }
        }
    }

    long long get_best() const {
        return best;
    }
};

class ButtonPresserBFS {
    std::vector<std::vector<int>> buttons;
    std::vector<int> target_state;
    long long best = std::numeric_limits<long long>::max();

    static std::string key_from_state(const std::vector<int>& state) {
        std::string key;
        key.reserve(state.size() * 3);
        for (size_t i = 0; i < state.size(); ++i) {
            key += std::to_string(state[i]);
            key += ',';
        }
        return key;
    }

public:
    ButtonPresserBFS(const std::vector<std::vector<int>>& buttons_, const std::vector<int>& target_state_)
        : buttons(buttons_), target_state(target_state_) {
        // BFS to find minimal button presses
        std::queue<std::pair<std::vector<int>, long long>> q;
        std::unordered_set<std::string> visited;

        std::vector<int> start(target_state.size(), 0);
        q.emplace(start, 0);
        visited.insert(key_from_state(start));

        while (!q.empty()) {
            auto [current_state, depth] = q.front();
            q.pop();

            if (current_state == target_state) {
                best = depth;
                break;
            }

            for (const auto& button : buttons) {
                bool overflow = false;
                std::vector<int> new_state = current_state;
                for (const auto& index : button) {
                    new_state[index] += 1;
                    if (new_state[index] > target_state[index]) {
                        overflow = true;
                        break;
                    }
                }
                if (overflow) continue;

                auto key = key_from_state(new_state);
                if (visited.find(key) == visited.end()) {
                    visited.insert(key);
                    q.emplace(std::move(new_state), depth + 1);
                }
            }
        }
    }

    long long get_best() const {
        return best;
    }
};

long long process2(std::vector<Machine>& machines) {
    const auto begin = std::chrono::high_resolution_clock::now();
    long long result = 0;
    std::cout << std::format("Processing {} machines\n", machines.size());
    for(int i = 0; i < machines.size(); ++i) {
        std::cout << std::format("Processing machine {}/{}\n", i + 1, machines.size());
        ButtonPresserBFS presser(machines[i].buttons, machines[i].joltage_requirement);
        const auto best = presser.get_best();
        std::cout << "Found solution with " << best << " button presses." << std::endl;
        result += best;
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Processing time: " << duration << " us" << std::endl;
    return result;
}

bool run_example1() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    auto machines = parse(data_content);
    auto result1 = process1(machines);
    std::cout << "Example1 result: " << result1 << std::endl;
    return result1 == 7;
}

bool run_data1() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    auto machines = parse(data_content);
    auto result1 = process1(machines);
    std::cout << "Data1 result: " << result1 << std::endl;
    return true;
}

bool run_example2() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    auto machines = parse(data_content);
    auto result1 = process2(machines);
    std::cout << "Example2 result: " << result1 << std::endl;
    return result1 == 33;
}

bool run_data2() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    auto machines = parse(data_content);
    auto result1 = process2(machines);
    std::cout << "Data2 result: " << result1 << std::endl;
    return true;
}

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
    run_data2();
    return 0;
}