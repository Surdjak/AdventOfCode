#include "file_utils.h"
#include "math_utils.h"
#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>
#include <array>
#include <unordered_map>
#include <map>

const char* DATA_FILEPATH = "./data/day9/data";
const char* EXAMPLE_FILEPATH = "./data/day9/example";
const char* TEST_FILEPATH = "./data/day9/test";

std::vector<std::array<long long, 2>> parse(const std::string& content) {
    std::vector<std::array<long long, 2>> tiles;
    std::istringstream ss(content);
    std::string line;
    while(std::getline(ss, line)) {
        // X,Y
        // std::cout << "Parsing line: " << line << std::endl;
        std::istringstream line_ss(line);
        std::string token;
        std::array<long long, 2> tile;
        int coord_index = 0;
        while(std::getline(line_ss, token, ',')) {
            tile[coord_index++] = std::stoll(token);
        }
        tiles.push_back(tile);
    }
    // bm.sort();
    return tiles;
}

long long process1(std::vector<std::array<long long, 2>>& tiles) {
    const auto begin = std::chrono::high_resolution_clock::now();
    long long result = 0;
    for (size_t i = 0; i < tiles.size(); ++i) {
        const auto& tile1 = tiles[i];
        for (size_t j = i + 1; j < tiles.size(); ++j) {
            const auto& tile2 = tiles[j];
            // Compute area of rectangle formed by tile1 and tile2
            long long dx = std::abs(tile1[0] - tile2[0]) + 1;
            long long dy = std::abs(tile1[1] - tile2[1]) + 1;
            long long area = dx * dy;
            if (area > result) {
                // std::cout << "Tiles " << i << " with coords (" << tile1.pos[0] << "," << tile1.pos[1] << ") and "
                //           << j << " with coords (" << tile2.pos[0] << "," << tile2.pos[1] << "):" << std::endl;
                // std::cout << "dx between tile " << i << " and tile " << j << " is " << dx << std::endl;
                // std::cout << "dy between tile " << i << " and tile " << j << " is " << dy << std::endl;
                // std::cout << "New largest area found: " << area << " between tiles " << i << " and " << j << std::endl;
                result = area;
            }
        }
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Processing time: " << duration << " us" << std::endl;
    return result;
}

long long process2(std::vector<std::array<long long, 2>>& tiles) {
    const auto begin = std::chrono::high_resolution_clock::now();
    long long result = 0;
    for (size_t i = 0; i < tiles.size(); ++i) {
        const auto& tile1 = tiles[i];
        for (size_t j = i + 1; j < tiles.size(); ++j) {
            const auto& tile2 = tiles[j];
            // Check if the rectangle formed by tile1 and tile2 fits within the polygon which corners are defined by the original tiles order
            std::vector<std::array<long long, 2>> rectangle_corners = {
                {tile1[0], tile1[1]}, {tile2[0], tile1[1]}, {tile2[0], tile2[1]}, {tile1[0], tile2[1]}
            };
            if (utils::rectangle_fits_in_polygon(tiles, rectangle_corners)) {
                // Compute area of rectangle formed by tile1 and tile2
                long long dx = std::abs(tile1[0] - tile2[0]) + 1;
                long long dy = std::abs(tile1[1] - tile2[1]) + 1;
                long long area = dx * dy;
                if (area > result) {
                    // std::cout << "Tiles " << i << " with coords (" << tile1[0] << "," << tile1[1] << ") and "
                    //           << j << " with coords (" << tile2[0] << "," << tile2[1] << "):" << std::endl;
                    // std::cout << "New largest area found: " << area << " between tiles " << i << " and " << j << std::endl;
                    result = area;
                }
            }
        }
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "Processing time: " << duration << " ms" << std::endl;
    return result;
}

bool run_example1() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    auto tiles = parse(data_content);
    auto result1 = process1(tiles);
    std::cout << "Example1 result: " << result1 << std::endl;
    return result1 == 50;
}

bool run_data1() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    auto tiles = parse(data_content);
    auto result1 = process1(tiles);
    std::cout << "Data1 result: " << result1 << std::endl;
    return true;
}

bool run_example2() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    auto tiles = parse(data_content);
    auto result1 = process2(tiles);
    std::cout << "Example2 result: " << result1 << std::endl;
    return result1 == 24;
}

bool run_data2() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    auto tiles = parse(data_content);
    auto result1 = process2(tiles);
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
    if(!run_example2()) {
        std::cerr << "Example2 failed!" << std::endl;
        return 1;
    }
    std::cout << "Example2 passed!" << std::endl;
    run_data2();
    return 0;
}