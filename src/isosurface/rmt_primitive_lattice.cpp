//
// Created by 赵润朔 on 2024/2/29.
//
#include <numbers>
#include "isosurface/rmt_primitive_lattice.h"

namespace rsmesh::isosurface {
    namespace detail {
        lattice_vectors::lattice_vectors()
                : base{{geometry::transform_vector(rotation(), {1.0, 1.0, -1.0}) / std::numbers::sqrt2,
                        geometry::transform_vector(rotation(), {1.0, -1.0, 1.0}) / std::numbers::sqrt2,
                        geometry::transform_vector(rotation(), {-1.0, 1.0, 1.0}) / std::numbers::sqrt2}} {}

        dual_lattice_vectors::dual_lattice_vectors()
                : base{{geometry::transform_vector(rotation(), {1.0, 1.0, 0.0}) / std::numbers::sqrt2,
                        geometry::transform_vector(rotation(), {1.0, 0.0, 1.0}) / std::numbers::sqrt2,
                        geometry::transform_vector(rotation(), {0.0, 1.0, 1.0}) / std::numbers::sqrt2}} {}

    } // namespace detail

    const detail::lattice_vectors LatticeVectors;

    const detail::dual_lattice_vectors DualLatticeVectors;
} // namespace rsmesh::isosurface