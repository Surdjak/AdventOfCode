#pragma once
#include <vector>
#include <iostream>

namespace utils {
    template<typename Container>
    void print_array(const Container& arr) {
        for (const auto& val : arr) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    template<typename Container>
    void print_array_2d(const Container& arr) {
        for (const auto& row : arr) {
            print_array(row);
        }
        std::cout << std::endl;
    }
}