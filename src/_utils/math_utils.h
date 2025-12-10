#include <type_traits>
#include <cmath>
#include <tuple>
#include <functional>

namespace std{
    namespace
    {
        // Code from boost
        // Reciprocal of the golden ratio helps spread entropy
        //     and handles duplicates.
        // See Mike Seymour in magic-numbers-in-boosthash-combine:
        //     http://stackoverflow.com/questions/4948780

        template <class T>
        inline void hash_combine(std::size_t& seed, T const& v)
        {
            seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }

        // Recursive template code derived from Matthieu M.
        template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
        struct HashValueImpl
        {
          static void apply(size_t& seed, Tuple const& tuple)
          {
            HashValueImpl<Tuple, Index-1>::apply(seed, tuple);
            hash_combine(seed, std::get<Index>(tuple));
          }
        };

        template <class Tuple>
        struct HashValueImpl<Tuple,0>
        {
          static void apply(size_t& seed, Tuple const& tuple)
          {
            hash_combine(seed, std::get<0>(tuple));
          }
        };
    }

    template <typename ... TT>
    struct hash<std::tuple<TT...>> 
    {
        size_t
        operator()(std::tuple<TT...> const& tt) const
        {                                              
            size_t seed = 0;                             
            HashValueImpl<std::tuple<TT...> >::apply(seed, tt);    
            return seed;                                 
        }                                              

    };

    template <typename T, typename U>
    struct hash<std::pair<T, U>> 
    {
        size_t
        operator()(std::pair<T, U> const& tt) const
        {                                              
            size_t seed = 0;                             
            hash_combine(seed, tt.first);                     
            hash_combine(seed, tt.second);
            return seed;                                 
        }                                              
    };
}

namespace utils {
    template<typename T>
    int integer_digits(T n) {
        static_assert(std::is_integral<T>::value, "digits function only supports integral types");
        if (n == 0) return 1;
        return static_cast<int>(std::floor(std::log10(std::abs(n)))) + 1;
    }

    template<typename T>
    double euclidean_distance(T x1, T y1, T z1, T x2, T y2, T z2) {
        return std::sqrt(
            (x2 - x1) * (x2 - x1) +
            (y2 - y1) * (y2 - y1) +
            (z2 - z1) * (z2 - z1)
        );
    }
}