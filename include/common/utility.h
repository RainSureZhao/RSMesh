//
// Created by RainSure on 2023/10/26.
//

#ifndef RSMESH_UTILITY_H
#define RSMESH_UTILITY_H

#include <functional>
#include <utility>

namespace rsmesh {
    namespace common {
        /**
         * @brief 用于比较两个对象的大小
         * @tparam T 模板参数，比较的两个对象的类型
         * @tparam Compare 模板参数，用于比较的比较函数类型，默认是std::less<T>
         * @param first 第一个对象
         * @param second 第二个对象
         * @param comp 比较函数
         * @return 返回一个pair，pair的第一个元素是较小的对象，第二个元素是较大的对象
         */
        template <class T, class Compare = std::less<T>>
        auto make_sorted_pair(T&& first, T&& second, Compare comp = Compare()) {
            return comp(first, second)
                   ? std::make_pair(std::forward<T>(first), std::forward<T>(second))
                   : std::make_pair(std::forward<T>(second), std::forward<T>(first));
        }
    } // common
} // rsmesh


#endif //RSMESH_UTILITY_H
