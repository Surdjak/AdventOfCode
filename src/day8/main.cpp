#include "file_utils.h"
#include "math_utils.h"
#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>
#include <array>
#include <unordered_map>
#include <map>

const char* DATA_FILEPATH = "./data/day8/data";
const char* EXAMPLE_FILEPATH = "./data/day8/example";
const char* TEST_FILEPATH = "./data/day8/test";

struct Box {
    std::array<long long, 3> pos;
    int circuit_id = -1;
};

struct BoxManager {
    std::vector<Box> boxes;
    std::unordered_map<std::pair<int, int>, double> distances_map;
    std::map<double, std::pair<int, int>> reverse_distances_map;
    std::map<int, std::vector<int>> circuits;
    int last_circuit_id = -1;

    void add_box(const Box& box) {
        for (int i = 0; i < boxes.size(); ++i) {
            const auto& other_box = boxes[i];
            double dist = utils::euclidean_distance(
                box.pos[0], box.pos[1], box.pos[2],
                other_box.pos[0], other_box.pos[1], other_box.pos[2]
            );
            distances_map[{i, static_cast<int>(boxes.size())}] = dist;
            reverse_distances_map[dist] = {i, static_cast<int>(boxes.size())};
        }
        boxes.push_back(box);
    }

    // void sort()
    // {
    //     // Sort reverse_distances_map by distance
    //     std::sort(
    //         reverse_distances_map.begin(),
    //         reverse_distances_map.end(),
    //         [](const auto& a, const auto& b) {
    //             return a.first < b.first;
    //         }
    //     );
    // }
};

BoxManager parse(const std::string& content) {
    BoxManager bm;
    std::istringstream ss(content);
    std::string line;
    while(std::getline(ss, line)) {
        // X,Y,Z
        // std::cout << "Parsing line: " << line << std::endl;
        std::istringstream line_ss(line);
        std::string token;
        Box box;
        int coord_index = 0;
        while(std::getline(line_ss, token, ',')) {
            box.pos[coord_index++] = std::stoll(token);
        }
        // std::cout << "Parsed box at (" << box.pos[0] << "," << box.pos[1] << "," << box.pos[2] << ")" << std::endl;
        bm.add_box(box);
    }
    // bm.sort();
    return bm;
}

template<int N>
std::pair<long long, long long> process1(BoxManager& bm, int multiply_largest_n) {
    const auto begin = std::chrono::high_resolution_clock::now();
    int merges = 0;
    int last_x_1;
    int last_x_2;
    for (const auto& distance : bm.reverse_distances_map) {
        std::cout << "Distance is " << distance.first << std::endl;
        const auto& box_indices = distance.second;
        int box1_id = box_indices.first;
        int box2_id = box_indices.second;
        Box& box1 = bm.boxes[box1_id];
        // std::cout << "Box 1 id: " << box1_id << ", circuit_id: " << box1.circuit_id << ", coordinates (" << box1.pos[0] << "," << box1.pos[1] << "," << box1.pos[2] << ")" << std::endl;
        Box& box2 = bm.boxes[box2_id];
        // std::cout << "Box 2 id: " << box2_id << ", circuit_id: " << box2.circuit_id << ", coordinates (" << box2.pos[0] << "," << box2.pos[1] << "," << box2.pos[2] << ")" << std::endl;
        if (box1.circuit_id == -1 && box2.circuit_id == -1) {
            int new_circuit_id = ++bm.last_circuit_id;
            box1.circuit_id = new_circuit_id;
            box2.circuit_id = new_circuit_id;
            bm.circuits[new_circuit_id] = {box1_id, box2_id};
            std::cout << "Created new circuit " << new_circuit_id << " with boxes " << box1_id << " and " << box2_id << std::endl;
        } else if (box1.circuit_id != -1 && box2.circuit_id == -1) {
            int circuit_id = box1.circuit_id;
            box2.circuit_id = circuit_id;
            bm.circuits[circuit_id].push_back(box2_id);
            std::cout << "Added box " << box2_id << " to circuit " << circuit_id << std::endl;
        } else if (box1.circuit_id == -1 && box2.circuit_id != -1) {
            int circuit_id = box2.circuit_id;
            box1.circuit_id = circuit_id;
            bm.circuits[circuit_id].push_back(box1_id);
            std::cout << "Added box " << box1_id << " to circuit " << circuit_id << std::endl;
        } else if (box1.circuit_id != box2.circuit_id) {
            // Keep largest circuit (circuit with more boxes)
            int circuit_id_to_keep = (bm.circuits[box1.circuit_id].size() >= bm.circuits[box2.circuit_id].size()) ? box1.circuit_id : box2.circuit_id;
            int circuit_id_to_merge = (circuit_id_to_keep == box1.circuit_id) ? box2.circuit_id : box1.circuit_id;
            for (int box_id : bm.circuits[circuit_id_to_merge]) {
                bm.boxes[box_id].circuit_id = circuit_id_to_keep;
                bm.circuits[circuit_id_to_keep].push_back(box_id);
            }
            std::cout << "Merged circuit " << circuit_id_to_merge << " into circuit " << circuit_id_to_keep << std::endl;
            bm.circuits.erase(circuit_id_to_merge);
        }
        if (bm.circuits.size() == 1 && bm.circuits.begin()->second.size() == bm.boxes.size()) {
            std::cout << "All boxes merged into a single circuit." << std::endl;
            last_x_1 = box1.pos[0];
            last_x_2 = box2.pos[0];
            break;
        }
        if (N > 0) {
            merges++;
            if (merges >= N) {
                last_x_1 = box1.pos[0];
                last_x_2 = box2.pos[0];
                break;
            }
        }
    }
    long long result = 1;
    std::vector<int> circuit_sizes;
    for (const auto& circuit : bm.circuits) {
        circuit_sizes.push_back(static_cast<int>(circuit.second.size()));
    }
    std::sort(circuit_sizes.begin(), circuit_sizes.end(), [](const auto& a, const auto& b) {
        return a > b;
    });
    for (int i = 0; i < multiply_largest_n && i < static_cast<int>(circuit_sizes.size()); ++i) {
        std::cout << "Circuit " << i << " size: " << circuit_sizes[i] << std::endl;
        result *= circuit_sizes[i];
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Processing time: " << duration << " us" << std::endl;
    return {result, last_x_1 * last_x_2};
}

bool run_example1() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    auto bm = parse(data_content);
    auto [result1, result2] = process1<10>(bm, 3);
    std::cout << "Example1 result: " << result1 << std::endl;
    return result1 == 40;
}

bool run_data1() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    auto bm = parse(data_content);
    auto [result1, result2] = process1<1000>(bm, 3);
    std::cout << "Data1 result: " << result1 << std::endl;
    return true;
}

bool run_example2() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    auto bm = parse(data_content);
    auto [result1, result2] = process1<-1>(bm, 3);
    std::cout << "Example2 result: " << result2 << std::endl;
    return result2 == 25272;
}

bool run_data2() {
    const auto data_content = utils::read_file(DATA_FILEPATH);
    auto bm = parse(data_content);
    auto [result1, result2] = process1<-1>(bm, 3);
    std::cout << "Data2 result: " << result2 << std::endl;
    return true;
}

int main() {
    if (!run_example1()) {
        std::cerr << "Example1 failed!" << std::endl;
        return 1;
    }
    std::cout << "Example1 passed!" << std::endl;
    run_data1();
    if(!run_example2()) {
        std::cerr << "Example2 failed!" << std::endl;
        return 1;
    }
    std::cout << "Example2 passed!" << std::endl;
    run_data2();
    return 0;
}