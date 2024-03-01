//
// Created by 赵润朔 on 2024/2/29.
//
#include "isosurface/rmt_node_list.h"

namespace rsmesh::isosurface {
    namespace detail {
        neighbor_cell_vectors::neighbor_cell_vectors()
                : base{{cell_vector(+1, +1, +1), cell_vector(+1, +1, +0), cell_vector(+0, +0, -1),
                        cell_vector(+1, +0, +1), cell_vector(+1, +0, +0), cell_vector(+0, -1, -1),
                        cell_vector(+0, -1, +0), cell_vector(-1, -1, -1), cell_vector(-1, -1, +0),
                        cell_vector(+0, +0, +1), cell_vector(-1, +0, -1), cell_vector(-1, +0, +0),
                        cell_vector(+0, +1, +1), cell_vector(+0, +1, +0)}} {}
    }
    const detail::neighbor_cell_vectors NeighborCellVectors;
} // namespace rsmesh::isosurface