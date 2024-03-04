//
// Created by RainSure on 2024/2/23.
//

#ifndef RSMESH_ISOSURFACE_TYPES_H
#define RSMESH_ISOSURFACE_TYPES_H

#include "Eigen/Core"
#include "boost/container_hash/hash.hpp"
#include <array>
#include <cstdint>
#include <functional>
#include "types.h"

namespace rsmesh::isosurface {
    using cell_vector = Eigen::Vector3i;

    using cell_vectors = Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor>;

    using vertex_index = index_t;

    using face = std::array<vertex_index, 3>;
}

template <>
struct std::hash<rsmesh::isosurface::cell_vector> {
    std::size_t operator()(const rsmesh::isosurface::cell_vector& cv) const noexcept {
        std::size_t seed{};
        boost::hash_combine(seed, std::hash<int>()(cv(0)));
        boost::hash_combine(seed, std::hash<int>()(cv(1)));
        boost::hash_combine(seed, std::hash<int>()(cv(2)));
        return seed;
    }
};


#endif //RSMESH_ISOSURFACE_TYPES_H
