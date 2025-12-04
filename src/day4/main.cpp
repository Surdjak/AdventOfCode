#include "file_utils.h"
#include "math_utils.h"
#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>

const char* DATA_FILEPATH = "./data/day4/data";
const char* EXAMPLE_FILEPATH = "./data/day4/example";

std::vector<std::vector<char>> parse(const std::string& content) {
    std::vector<std::vector<char>> grid;
    std::istringstream stream(content);
    std::string line;
    while (std::getline(stream, line)) {
        // std::cout << "Parsing line: " << line << std::endl;
        std::vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        grid.push_back(row);
    }
    // std::cout << "Parsed grid with " << grid.size() << " rows and " << (grid.empty() ? 0 : grid[0].size()) << " columns." << std::endl;
    return grid;
}

int process1(const std::vector<std::vector<char>>& grid) {
    const auto begin = std::chrono::high_resolution_clock::now();
    int result = 0;
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[i].size(); ++j) {
            // Example processing: count number of '#' characters
            // std::cout << "Processing cell (" << i << ", " << j << "): " << grid[i][j] << std::endl;
            if (grid[i][j] == '@') {
                // std::cout << "Found '@' at (" << i << ", " << j << ")" << std::endl;
                int local_neighbors = 0;
                for(int di = -1; di <= 1; ++di) {
                    for(int dj = -1; dj <= 1; ++dj) {
                        if(di == 0 && dj == 0) continue;
                        int ni = i + di;
                        int nj = j + dj;
                        if(ni >= 0 && ni < grid.size() && nj >= 0 && nj < grid[i].size()) {
                            if(grid[ni][nj] == '@') {
                                local_neighbors++;
                            }
                        }
                    }
                }
                // std::cout << "Local neighbors: " << local_neighbors << std::endl;
                if (local_neighbors < 4) {
                    result++;
                }
            }
        }
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Processing time: " << duration << " us" << std::endl;
    return result;
}

int process2(std::vector<std::vector<char>> grid) {
    const auto begin = std::chrono::high_resolution_clock::now();
    int result = 0;
    bool working = true;
    while(working)
    {
        working = false;
        std::vector<std::vector<char>> new_grid;
        new_grid = grid;
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[i].size(); ++j) {
                // Example processing: count number of '#' characters
                // std::cout << "Processing cell (" << i << ", " << j << "): " << grid[i][j] << std::endl;
                if (grid[i][j] == '@') {
                    // std::cout << "Found '@' at (" << i << ", " << j << ")" << std::endl;
                    int local_neighbors = 0;
                    for(int di = -1; di <= 1; ++di) {
                        for(int dj = -1; dj <= 1; ++dj) {
                            if(di == 0 && dj == 0) continue;
                            int ni = i + di;
                            int nj = j + dj;
                            if(ni >= 0 && ni < grid.size() && nj >= 0 && nj < grid[i].size()) {
                                if(grid[ni][nj] == '@') {
                                    local_neighbors++;
                                }
                            }
                        }
                    }
                    // std::cout << "Local neighbors: " << local_neighbors << std::endl;
                    if (local_neighbors < 4) {
                        new_grid[i][j] = '.';
                        working = true;
                        result++;
                    }
                }
            }
        }
        grid = new_grid;
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Processing time: " << duration << " us" << std::endl;
    return result;
}

bool run_example1() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    const auto grid = parse(data_content);
    long long result = process1(grid);
    std::cout << "Example1 result: " << result << std::endl;
    return result == 13;
}

bool run_data1() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    const auto grid = parse(data_content);
    long long result = process1(grid);
    std::cout << "Data1 result: " << result << std::endl;
    return true;
}

bool run_example2() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    auto grid = parse(data_content);
    long long result = process2(grid);
    std::cout << "Example2 result: " << result << std::endl;
    return result == 43;
}

bool run_data2() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    auto grid = parse(data_content);
    long long result = process2(grid);
    std::cout << "Data2 result: " << result << std::endl;
    return true;
}

int main() {
    // run_example1();
    // run_data1();
    // run_example2();
    run_data2();
    return 0;
}