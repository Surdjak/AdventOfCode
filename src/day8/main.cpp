#include "file_utils.h"
#include "math_utils.h"
#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>
#include <array>
#include <unordered_map>

const char* DATA_FILEPATH = "./data/day7/data";
const char* EXAMPLE_FILEPATH = "./data/day7/example";
const char* TEST_FILEPATH = "./data/day7/test";

struct Box {
    std::array<long long, 3> pos;
    int circuit_id = -1;
};

struct BoxManager {
    std::vector<Box> boxes;
    std::unordered_map<std::pair<int, int>, double> distances_map;
    std::unordered_map<double, std::pair<int, int>> reverse_distances_map;
    std::unordered_map<int, std::vector<int>> circuits;
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
};

BoxManager parse(const std::string& content) {
    BoxManager bm;
    std::istringstream ss(content);
    std::string line;
    while(std::getline(ss, line)) {
        // X,Y,Z
        std::istringstream line_ss(line);
        std::string token;
        Box box;
        int coord_index = 0;
        while(std::getline(line_ss, token, ',')) {
            box.pos[coord_index++] = std::stoll(token);
        }
        bm.add_box(box);
    }
    return bm;
}

long long process1(BoxManager& bm) {
    const auto begin = std::chrono::high_resolution_clock::now();
    long long result = 0;
    for (const auto& distance : bm.reverse_distances_map) {
        const auto& box_indices = distance.second;
        int box1_id = box_indices.first;
        int box2_id = box_indices.second;
        Box& box1 = bm.boxes[box1_id];
        Box& box2 = bm.boxes[box2_id];
        if (box1.circuit_id == -1 && box2.circuit_id == -1) {
            int new_circuit_id = ++bm.last_circuit_id;
            box1.circuit_id = new_circuit_id;
            box2.circuit_id = new_circuit_id;
            bm.circuits[new_circuit_id] = {box1_id, box2_id};
        } else if (box1.circuit_id != -1 && box2.circuit_id == -1) {
            int circuit_id = box1.circuit_id;
            box2.circuit_id = circuit_id;
            bm.circuits[circuit_id].push_back(box2_id);
        } else if (box1.circuit_id == -1 && box2.circuit_id != -1) {
            int circuit_id = box2.circuit_id;
            box1.circuit_id = circuit_id;
            bm.circuits[circuit_id].push_back(box1_id);
        } else if (box1.circuit_id != box2.circuit_id) {
            int circuit_id_to_keep = box1.circuit_id;
            int circuit_id_to_merge = box2.circuit_id;
            for (int b_id : bm.circuits[circuit_id_to_merge]) {
                bm.boxes[b_id].circuit_id = circuit_id_to_keep;
                bm.circuits[circuit_id_to_keep].push_back(b_id);
            }
            bm.circuits.erase(circuit_id_to_merge);
        }
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Processing time: " << duration << " us" << std::endl;
    return result;
}

bool run_example1() {
    const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
    auto bm = parse(data_content);
    long long result = process1(bm);
    std::cout << "Example1 result: " << result << std::endl;
    return result == 40;
}

// bool run_data1() {
//     const auto data_content = utils::read_file(DATA_FILEPATH);
//     auto diagram = parse(data_content);
//     long long result = process1(diagram);
//     std::cout << "Data1 result: " << result << std::endl;
//     return true;
// }

// bool run_example2() {
//     const auto data_content = utils::read_file(EXAMPLE_FILEPATH);
//     auto diagram = parse(data_content);
//     long long result = process2(diagram);
//     std::cout << "Example2 result: " << result << std::endl;
//     return result == 40;
// }

// bool run_data2() {
//     const auto data_content = utils::read_file(DATA_FILEPATH);
//     auto diagram = parse(data_content);
//     long long result = process2(diagram);
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
    if(!run_example2()) {
        std::cerr << "Example2 failed!" << std::endl;
        return 1;
    }
    std::cout << "Example2 passed!" << std::endl;
    run_data2();
    return 0;
}