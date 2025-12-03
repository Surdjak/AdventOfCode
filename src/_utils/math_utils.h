#include <type_traits>
#include <cmath>

namespace utils {
    template<typename T>
    int integer_digits(T n) {
        static_assert(std::is_integral<T>::value, "digits function only supports integral types");
        if (n == 0) return 1;
        return static_cast<int>(std::floor(std::log10(std::abs(n)))) + 1;
    }
}