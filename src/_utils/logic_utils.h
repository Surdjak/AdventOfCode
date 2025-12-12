#pragma once
#include <vector>
#include <array>

namespace utils {
    template<typename T>
    std::vector<std::vector<int>> get_permutations_with_repeat(const int size, const std::vector<T>& possible_elements) {
        // Generate all permutations of N elements from possible_elements with possible repetitions
        const int total_permutations = static_cast<int>(pow(possible_elements.size(), size));
        std::vector<std::vector<T>> permutations(total_permutations);
        for (auto& perm : permutations) {
            perm.resize(size);
        }
        for (int i = 0; i < total_permutations; ++i) {
            int index = i;
            for (int j = 0; j < size; ++j) {
                permutations[i][j] = possible_elements[index % possible_elements.size()];
                index /= possible_elements.size();
            }
        }
        return permutations;
    }
}