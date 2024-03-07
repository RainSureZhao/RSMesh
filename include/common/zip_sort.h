//
// Created by RainSure on 2023/11/11.
//

#ifndef RSMESH_ZIP_SORT_H
#define RSMESH_ZIP_SORT_H

#include <vector>
#include <iterator>
#include <type_traits>
#include <common/macros.h>
#include <numeric>


namespace rsmesh {
    namespace common {
        namespace detail {
        /**
         * @brief 根据提供的排列数组对序列进行逆置换。
         *
         * 此函数接受一个随机访问迭代器范围 [begin, end) 和一个排列数组 p，然后根据 p 中的索引对
         * 序列进行逆置换。逆置换意味着如果 p[i] = j，则在排列后的序列中，原始位置为 i 的元素将移动到
         * 位置 j。这个过程是原地执行的，不需要额外的存储空间。
         *
         * 注意：为了正确执行逆置换，排列数组 p 必须是一个有效的排列，即包含从 0 到 n-1（n 是序列的大小）的
         * 每个数字恰好一次。
         *
         * @tparam RandomAccessIterator 迭代器类型，应支持随机访问。这通常意味着函数可以用于数组、
         *         std::vector 等容器的迭代器。
         * @tparam D 排列数组中索引的类型，默认为迭代器的差类型（difference_type）。
         * @param begin 序列的开始迭代器。
         * @param end 序列的结束迭代器。
         * @param p 包含逆置换索引的向量。p 中的每个值表示对应位置的元素在置换后应该在的新位置。
         *
         * @code
         * std::vector<int> v = {1, 2, 3, 4, 5};
         * std::vector<size_t> p = {2, 0, 4, 1, 3}; // 逆置换后的位置
         * inverse_permute(v.begin(), v.end(), p);
         * // v 现在是 {3, 1, 5, 2, 4}
         * @endcode
         */
            template <class RandomAccessIterator, 
                    class D = typename std::iterator_traits<RandomAccessIterator>::difference_type>
            static void inverse_permute(RandomAccessIterator begin, RandomAccessIterator end,
                                        const std::vector<D>& p) {
                using std::swap;
                auto size = std::distance(begin, end);
                
                std::vector<bool> done(size);
                for(D i = 0; i < size; i ++) {
                    if(done.at(i)) {
                        continue;
                    }
                    
                    auto prev_j = i;
                    auto j = p.at(i);
                    
                    while(i != j) {
                        swap(begin[prev_j], begin[j]);
                        done.at(j) = true;
                        prev_j = j;
                        j = p.at(j);
                    }
                    done.at(i) = true;
                }
            }
        } // namespace detail

        /**
         * @brief 同时对两个序列进行排序，保持它们之间的相对顺序。
         *
         * `zip_sort`函数接受两个序列（通过它们的迭代器定义）以及一个比较函数`compare`。
         * 它将这两个序列“压缩”在一起，然后根据提供的比较函数对它们进行排序。排序完成后，
         * 两个序列中相对应的元素将保持相同的顺序。这意味着，如果在第一个序列中元素A在元素B之前，
         * 那么在第二个序列中，与元素A相对应的元素也会在与元素B相对应的元素之前。
         *
         * 这种排序特别有用于需要保持两个相关序列同步排序的情况，例如，一个序列包含了值，另一个序列包含了对应的索引或键。
         *
         * @tparam RandomAccessIterator1 第一个序列的迭代器类型。
         * @tparam RandomAccessIterator2 第二个序列的迭代器类型。
         * @tparam Compare 比较函数的类型。该函数应该能接受两个`std::pair`作为参数，每个`pair`包含两个序列中的相对应元素，返回一个布尔值。
         * @param begin1 第一个序列的开始迭代器。
         * @param end1 第一个序列的结束迭代器。
         * @param begin2 第二个序列的开始迭代器。
         * @param end2 第二个序列的结束迭代器。
         * @param compare 比较函数，用于决定排序顺序。
         *
         * @note 这个函数要求两个序列具有相同的长度，并且它们的迭代器具有相同的`difference_type`。
         *       如果这些要求不满足，函数将编译失败或抛出断言。
         *
         * @code
         * std::vector<int> values = {4, 2, 5, 3, 1};
         * std::vector<int> indices = {0, 1, 2, 3, 4};
         * auto compare = [](const auto& a, const auto& b) { return a.first < b.first; };
         * zip_sort(values.begin(), values.end(), indices.begin(), indices.end(), compare);
         * // 此时，values = {1, 2, 3, 4, 5}, indices = {4, 1, 3, 0, 2}
         * @endcode
         */
        template <class RandomAccessIterator1, class RandomAccessIterator2, class Compare>
        void zip_sort(RandomAccessIterator1 begin1, RandomAccessIterator1 end1,
                      RandomAccessIterator2 begin2, RandomAccessIterator2 end2, Compare compare) {
            using D1 = typename std::iterator_traits<RandomAccessIterator1>::difference_type;
            using D2 = typename std::iterator_traits<RandomAccessIterator2>::difference_type;

            static_assert(std::is_same<D1, D2>::value,
                          "RandomAccessIterator1 and RandomAccessIterator2 must have the same difference_type.");
            RSMESH_ASSERT(std::distance(begin1, end1) == std::distance(begin2, end2));

            auto size = std::distance(begin1, end1);

            std::vector<D1> permutation(size);
            std::iota(begin(permutation), end(permutation), 0);
            std::sort(begin(permutation), end(permutation), [&](int i, int j) {
                return compare(std::make_pair(begin1[i], begin2[i]), std::make_pair(begin1[j], begin2[j]));
            });

            detail::inverse_permute(begin1, end1, permutation);
            detail::inverse_permute(begin2, end2, permutation);
        }
    }

    
}

#endif //RSMESH_ZIP_SORT_H
