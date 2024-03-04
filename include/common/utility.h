//
// Created by RainSure on 2023/10/26.
//

#ifndef RSMESH_UTILITY_H
#define RSMESH_UTILITY_H

#include <functional>
#include <utility>

namespace rsmesh {
    namespace common {
        template <class T, class Compare = std::less<T>>
        auto make_sorted_pair(T&& first, T&& second, Compare comp = Compare()) {
            return comp(first, second)
                   ? std::make_pair(std::forward<T>(first), std::forward<T>(second))
                   : std::make_pair(std::forward<T>(second), std::forward<T>(first));
        }
    } // common
} // rsmesh


#endif //RSMESH_UTILITY_H
