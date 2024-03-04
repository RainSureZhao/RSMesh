//
// Created by RainSure on 2024/2/4.
//

#ifndef RSMESH_FMM_TREE_HEIGHT_H
#define RSMESH_FMM_TREE_HEIGHT_H
#include <algorithm>
#include <cmath>
#include "types.h"

namespace rsmesh::fmm {
    inline int fmm_tree_height(index_t points_estimated) {
        return 2 + std::max(2, static_cast<int>(std::floor(std::log(points_estimated))));
    }
}// namespace rsmesh::fmm

#endif //RSMESH_FMM_TREE_HEIGHT_H
