//
// Created by RainSure on 2023/11/10.
//

#ifndef RSMESH_ORTHONORMALIZE_H
#define RSMESH_ORTHONORMALIZE_H

#include "Eigen/Core"
#include "types.h"

namespace rsmesh {
    namespace common {
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
