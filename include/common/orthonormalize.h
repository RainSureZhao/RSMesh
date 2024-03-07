//
// Created by RainSure on 2023/11/10.
//

#ifndef RSMESH_ORTHONORMALIZE_H
#define RSMESH_ORTHONORMALIZE_H

#include "Eigen/Core"
#include "types.h"

namespace rsmesh {
    namespace common {
        /**
         * @brief 对矩阵的列进行正交化
         *
         * 本函数使用Gram-Schmidt正交化过程对输入矩阵的列进行正交化处理。正交化处理后，
         * 矩阵的每一列将成为单位向量，并且列与列之间正交。这是线性代数中常见的需求，特别是
         * 在处理涉及基变换、矩阵分解等问题时。
         *
         * @see [Gram-Schmidt Process](https://en.wikipedia.org/wiki/Gram–Schmidt_process)
         *
         * @tparam Derived 是Eigen库中矩阵或者向量表达式的类型。这使得函数能以泛型的方式
         *                 应用于各种不同类型的Eigen矩阵和表达式上，增加了代码的通用性和灵活性。
         * @param m        是对Eigen::MatrixBase的引用，代表了要被正交化处理的矩阵。它既是输入也是输出参数；
         *                 函数开始时，`m`包含原始数据，函数执行结束后，`m`中的数据将被更新为正交化后的矩阵列。
         *
         * @code
         * Eigen::Matrix3d m;
         * m << 1, 2, 3,
         *     4, 5, 6,
         *     7, 8, 9;
         * orthonormalize_cols(m);
         * @endcode
         */
        template <class Derived>
        void orthonormalize_cols(Eigen::MatrixBase<Derived>& m) {
            
            auto n = m.cols();
            for(index_t i = 0; i < n; i ++) {
                m.col(i) /= m.col(i).norm();
                for(index_t j = i + 1; j < n; j ++) {
                    m.col(j) -= m.col(i).dot(m.col(j)) * m.col(j);
                }
            }
        }
    }
}

#endif //RSMESH_ORTHONORMALIZE_H
