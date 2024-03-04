//
// Created by RainSure on 2024/2/23.
//

#ifndef RSMESH_BIT_H
#define RSMESH_BIT_H

#include <bit>

/**
 * \namespace rsmesh::isosurface
 * \brief 该命名空间下主要定义了等值面提取相关的类和函数
 */
namespace rsmesh::isosurface {
    template <class T>
    int bit_count(T bit_set) {
        return std::popcount(bit_set);
    }

    template <class T>
    int bit_peek(T bit_set) {
        if (bit_set == 0) {
            return -1;
        }

        return std::countr_zero(bit_set);
    }

    template <class T>
    int bit_pop(T* bit_set) {
        if (*bit_set == 0) {
            return -1;
        }

        auto bit_index = bit_peek(*bit_set);
        auto bit = T{1} << bit_index;
        *bit_set ^= bit;

        return bit_index;
    }
}

#endif //RSMESH_BIT_H
