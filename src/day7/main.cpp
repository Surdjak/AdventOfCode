#include "file_utils.h"
#include "math_utils.h"
#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>
#include <unordered_set>

const char* DATA_FILEPATH = "./data/day7/data";
const char* EXAMPLE_FILEPATH = "./data/day7/example";
const char* TEST_FILEPATH = "./data/day7/test";

struct Tachyon {
    unsigned int id;
    unsigned short row;
    unsigned short col;
    unsigned long long charge;
};

struct TachyonDiagram {
    int height;
    std::pair<int, int> start;
    std::vector<std::pair<int, int>> splitters;
    std::vector<std::vector<bool>> splitters_per_col;

    std::vector<std::vector<char>> debug_grid;
    
    void init_debug_grid() {
        int rightest_col = 0;
        for (const auto& splitter : splitters) {
            if (splitter.second > rightest_col) {
                rightest_col = splitter.second;
            }
        }
        debug_grid = std::vector<std::vector<char>>(height, std::vector<char>(rightest_col + 2, '.'));
        for (const auto& splitter : splitters) {
            debug_grid[splitter.first][splitter.second] = '^';
        }
        debug_grid[start.first][start.second] = 'S';
    }

    void print_debug_grid() const {
        for (const auto& row : debug_grid) {
            for (const auto& c : row) {
                std::cout << c;
            }
            std::cout << std::endl;
        }
    }
};

TachyonDiagram parse(const std::string& content) {
    TachyonDiagram diagram;
    std::istringstream stream(content);
    std::string line;
    int line_index = 0;
    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        for (int char_index = 0; char_index < line.size(); ++char_index) {
            if (line[char_index] == 'S') {
                diagram.start = std::make_pair(line_index, char_index);
            } else if (line[char_index] == '^') {
                diagram.splitters.emplace_back(line_index, char_index);
            }
        }
        line_index++;
    }
    diagram.height = line_index;
    int rightest_col = 0;
    for (const auto& splitter : diagram.splitters) {
        if (splitter.second > rightest_col) {
            rightest_col = splitter.second;
        }
    }
    diagram.splitters_per_col = std::vector<std::vector<bool>>(rightest_col + 2, std::vector<bool>(diagram.height+2, false));
    for (const auto& splitter : diagram.splitters) {
        diagram.splitters_per_col[splitter.second][splitter.first] = true;
    }
    std::cout << "Parsed " << diagram.splitters.size() << " splitters." << std::endl;
    std::cout << "Diagram height: " << diagram.height << std::endl;
    std::cout << "Start position: (" << diagram.start.first << ", " << diagram.start.second << ")" << std::endl;
    return diagram;
}

long long process1(TachyonDiagram& diagram) {
    const auto begin = std::chrono::high_resolution_clock::now();
    // diagram.init_debug_grid();
    long long result = 0;
    std::vector<Tachyon> tachyons = {{0, static_cast<unsigned short>(diagram.start.first), static_cast<unsigned short>(diagram.start.second)}};
    std::vector<Tachyon> new_tachyons = {};
    unsigned int tachyon_id_counter = 1;
    while(tachyons.size() > 0) {
        // std::cout << "===============================" << std::endl;
        for (int i = 0; i < tachyons.size(); ++i) {
            auto& tachyon = tachyons[i];
            bool split_occurred = false;
            for (const auto& splitter : diagram.splitters) {
                if(tachyon.row+1 == splitter.first && tachyon.col == splitter.second) {
                    split_occurred = true;
                    // std::cout << "Tachyon " << tachyon.id << " split at (" << splitter.first << ", " << splitter.second << ")" << std::endl;
                    result++;
                    bool left_exists = false;
                    bool right_exists = false;
                    for(const auto& t : new_tachyons) {
                        if (t.col == splitter.second-1) {
                            left_exists = true;
                        } else if (t.col == splitter.second+1) {
                            right_exists = true;
                        }
                    }
                    if (!left_exists) {
                        // std::cout << "Adding left tachyon " << tachyon_id_counter << " at (" << tachyon.row+1 << ", " << tachyon.col-1 << ")" << std::endl;
                        new_tachyons.emplace_back(tachyon_id_counter++, tachyon.row+1, tachyon.col-1);
                        // diagram.debug_grid[new_tachyons.back().row][new_tachyons.back().col] = '|';
                    } 
                    if (!right_exists) {
                        // std::cout << "Adding right tachyon " << tachyon_id_counter << " at (" << tachyon.row+1 << ", " << tachyon.col+1 << ")" << std::endl;
                        new_tachyons.emplace_back(tachyon_id_counter++, tachyon.row+1, tachyon.col+1);
                        // diagram.debug_grid[new_tachyons.back().row][new_tachyons.back().col] = '|';
                    }
                    break;
                }
            }
            if (!split_occurred) {
                bool down_exists = false;
                for(const auto& t : new_tachyons) {
                    if (t.row == tachyon.row+1 && t.col == tachyon.col) {
                        down_exists = true;
                        break;
                    }
                }
                if(!down_exists && tachyon.row + 1 < diagram.height) {
                    // std::cout << "Moving down tachyon " << tachyon.id << " to (" << tachyon.row + 1 << ", " << tachyon.col << ")" << std::endl;
                    new_tachyons.emplace_back(tachyon.id, tachyon.row + 1, tachyon.col);
                    // diagram.debug_grid[new_tachyons.back().row][new_tachyons.back().col] = '|';
                }
            }
        }
        tachyons = std::move(new_tachyons);
        new_tachyons.clear();
        // diagram.print_debug_grid();
        // std::cout << "Tachyons count: " << tachyons.size() << std::endl;
        // std::cout << "Current result: " << result << std::endl;
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Processing time: " << duration << " us" << std::endl;
    return result;
}

long long process2(TachyonDiagram& diagram) {
    const auto begin = std::chrono::high_resolution_clock::now();
    long long result = 0;
    std::vector<Tachyon> tachyons = {{0, static_cast<unsigned short>(diagram.start.first), static_cast<unsigned short>(diagram.start.second), 1}};
    std::vector<Tachyon> new_tachyons = {};
    unsigned int tachyon_id_counter = 1;
    unsigned int lowest_row_reached = diagram.start.first;
    while(tachyons.size() > 0) {
        for (int i = 0; i < tachyons.size(); ++i) {
            auto& tachyon = tachyons[i];
            // std::cout << "Processing tachyon " << tachyon.id << " (" << i << ") at (" << tachyon.row << ", " << tachyon.col << ")" << std::endl;
            if(diagram.splitters_per_col[tachyon.col][tachyon.row+2]) {
                // std::cout << "Tachyon " << tachyon.id << " split at (" << tachyon.row+2 << ", " << tachyon.col << ")" << std::endl;
                bool left_exists = false;
                bool right_exists = false;
                for(auto& t : new_tachyons) {
                    if (t.col == tachyon.col-1) {
                        left_exists = true;
                        t.charge += tachyon.charge;
                        // std::cout << "Adding charge of " << tachyon.charge << " to existing left tachyon " << t.id << ". New charge: " << t.charge << std::endl;
                    } else if (t.col == tachyon.col+1) {
                        right_exists = true;
                        t.charge += tachyon.charge;
                        // std::cout << "Adding charge of " << tachyon.charge << " to existing right tachyon " << t.id << ". New charge: " << t.charge << std::endl;
                    }
                    if (left_exists && right_exists) {
                        break;
                    }
                }
                if (!left_exists) {
                    // std::cout << "Adding left tachyon " << tachyon_id_counter << " at (" << tachyon.row+2 << ", " << tachyon.col-1 << ")" << std::endl;
                    new_tachyons.emplace_back(tachyon_id_counter++, tachyon.row+2, tachyon.col-1, tachyon.charge);
                }
                if (!right_exists) {
                    // std::cout << "Adding right tachyon " << tachyon_id_counter << " at (" << tachyon.row+2 << ", " << tachyon.col+1 << ")" << std::endl;
                    new_tachyons.emplace_back(tachyon_id_counter++, tachyon.row+2, tachyon.col+1, tachyon.charge);
                }
            } else {
                bool down_exists = false;
                for(auto& t : new_tachyons) {
                    if (t.col == tachyon.col) {
                        down_exists = true;
                        t.charge += tachyon.charge;
                        // std::cout << "Adding charge of " << tachyon.charge << " to existing down tachyon " << t.id << ". New charge: " << t.charge << std::endl;
                        break;
                    }
                }
                if (!down_exists) {
                    if(tachyon.row + 2 < diagram.height) {
                        // std::cout << "Moving down tachyon " << tachyon.id << " to (" << tachyon.row + 2 << ", " << tachyon.col << ")" << std::endl;
                        new_tachyons.emplace_back(tachyon.id, tachyon.row + 2, tachyon.col, tachyon.charge);
                    } else {
                        // std::cout << "Tachyon " << tachyon.id << " reached the bottom with charge " << tachyon.charge << std::endl;
                        result+=tachyon.charge;
                        // result+=1;
                    }
                }
            }
        }
        lowest_row_reached += 2;
        tachyons = std::move(new_tachyons);
        new_tachyons.clear();
        // std::cout << "Tachyons count: " << tachyons.size() << std::endl;
        // std::cout << "Tachyons row pos: " << lowest_row_reached << std::endl;
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Processing time: " << duration << " us" << std::endl;
    return result;
}

bool run_example1() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    auto diagram = parse(data_content);
    long long result = process1(diagram);
    std::cout << "Example1 result: " << result << std::endl;
    return result == 21;
}

bool run_data1() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    auto diagram = parse(data_content);
    long long result = process1(diagram);
    std::cout << "Data1 result: " << result << std::endl;
    return true;
}

bool run_example2() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    auto diagram = parse(data_content);
    long long result = process2(diagram);
    std::cout << "Example2 result: " << result << std::endl;
    return result == 40;
}

bool run_data2() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    auto diagram = parse(data_content);
    long long result = process2(diagram);
    std::cout << "Data2 result: " << result << std::endl;
    return true;
}

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