#pragma once
#include <algorithm>
#include <type_traits>
#include <cmath>
#include <tuple>
#include <functional>
#include <vector>
#include <array>
#include <iostream>

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

    template<typename T>
    long double cross(
        const std::array<T, 2>& a,
        const std::array<T, 2>& b,
        const std::array<T, 2>& c
    ) {
        // (b - a) x (c - a)
        long double abx = static_cast<long double>(b[0])  - static_cast<long double>(a[0]);
        long double aby = static_cast<long double>(b[1]) - static_cast<long double>(a[1]);
        long double acx = static_cast<long double>(c[0])  - static_cast<long double>(a[0]);
        long double acy = static_cast<long double>(c[1]) - static_cast<long double>(a[1]);
        return abx * acy - aby * acx;
    }

    // Helper: check if point lies on segment [a,b]
    template<typename T>
    bool point_on_segment(
        const std::array<T, 2>& a,
        const std::array<T, 2>& b,
        const std::array<T, 2>& p,
        long double eps = 1e-12L
    ) {
        long double cr = std::fabsl(cross(a, b, p));
        if (cr > eps) {
            // std::cout << "Not colinear" << std::endl;
            return false; // not collinear
        }

        long double px = static_cast<long double>(p[0]);
        long double py = static_cast<long double>(p[1]);
        long double ax = static_cast<long double>(a[0]);
        long double ay = static_cast<long double>(a[1]);
        long double bx = static_cast<long double>(b[0]);
        long double by = static_cast<long double>(b[1]);

        bool colinear = (px >= std::min(ax, bx) - eps && px <= std::max(ax, bx) + eps && py >= std::min(ay, by) - eps && py <= std::max(ay, by) + eps);
        // std::cout << std::format("Colinear: {}", colinear) << std::endl;

        return colinear;
    }

    template<typename T>
    bool point_in_polygon(
        const std::vector<std::array<T, 2>>& polygon,
        const std::array<T, 2>& point
    ) {
        const size_t n = polygon.size();
        if (n < 3) {
            // std::cout << "Polygon has less than 3 vertices." << std::endl;
            return false;
        }

        // 1) Boundary check: point on any edge ⇒ inside
        for (size_t i = 0, j = n - 1; i < n; j = i++) {
            if (point_on_segment(polygon[j], polygon[i], point))
                return true;
        }

        // 2) Standard ray-casting (strict interior only)
        bool inside = false;
        long double x  = static_cast<long double>(point[0]);
        long double y  = static_cast<long double>(point[1]);

        for (size_t i = 0, j = n - 1; i < n; j = i++) {
            long double xi = static_cast<long double>(polygon[i][0]);
            long double yi = static_cast<long double>(polygon[i][1]);
            long double xj = static_cast<long double>(polygon[j][0]);
            long double yj = static_cast<long double>(polygon[j][1]);

            bool intersect = ((yi > y) != (yj > y)) &&
                            (x < (xj - xi) * (y - yi) / (yj - yi) + xi);
            if (intersect)
                inside = !inside;
        }

        return inside;
    }

    template<typename T>
    bool rectangle_fits_in_polygon(
        const std::vector<std::array<T, 2>>& polygon,
        const std::vector<std::array<T, 2>>& rectangle
    ) {
        for (const auto& corner : rectangle) {
            if (!point_in_polygon(polygon, corner)) {
                // std::cout << "Corner (" << corner[0] << "," << corner[1] << ") is outside the polygon." << std::endl;
                return false;
            }
        }
        const std::array<T, 2> center = {
            (rectangle[0][0] + rectangle[2][0]) / 2,
            (rectangle[0][1] + rectangle[2][1]) / 2
        };
        if (!point_in_polygon(polygon, center)) {
            // std::cout << "Center (" << center[0] << "," << center[1] << ") is outside the polygon." << std::endl;
            return false;
        }
        // If any edge of the rectangle intersects with any edge of the polygon, return false
        // Otherwise, return true
        for(int i = 0; i < rectangle.size(); ++i) {
            const auto& rect_start = rectangle[i];
            const auto& rect_end = rectangle[(i + 1) % rectangle.size()];
            for(int j = 0; j < polygon.size(); ++j) {
                const auto& poly_start = polygon[j];
                const auto& poly_end = polygon[(j + 1) % polygon.size()];
                // Check if line segments (rect_start, rect_end) and (poly_start, poly_end) intersect
                long long d1 = (rect_end[0] - rect_start[0]) * (poly_start[1] - rect_start[1]) -
                         (rect_end[1] - rect_start[1]) * (poly_start[0] - rect_start[0]);
                long long d2 = (rect_end[0] - rect_start[0]) * (poly_end[1] - rect_start[1]) -
                         (rect_end[1] - rect_start[1]) * (poly_end[0] - rect_start[0]);
                long long d3 = (poly_end[0] - poly_start[0]) * (rect_start[1] - poly_start[1]) -
                         (poly_end[1] - poly_start[1]) * (rect_start[0] - poly_start[0]);
                long long d4 = (poly_end[0] - poly_start[0]) * (rect_end[1] - poly_start[1]) -
                         (poly_end[1] - poly_start[1]) * (rect_end[0] - poly_start[0]);
                if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
                    ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
                        // std::cout << "Rectangle edge ((" << rect_start[0] << "," << rect_start[1] << ") to ("
                        //         << rect_end[0] << "," << rect_end[1] << ")) intersects with polygon edge (("
                        //         << poly_start[0] << "," << poly_start[1] << ") to ("
                        //         << poly_end[0] << "," << poly_end[1] << "))." << std::endl;
                    return false;
                }
            }
        }
        return true;
    }
}