//
// Created by RainSure on 2024/2/23.
//

#ifndef RSMESH_MESH_DEFECTS_FINDER_H
#define RSMESH_MESH_DEFECTS_FINDER_H

#include "geometry/point3d.h"
#include "isosurface/isosurface_types.h"
#include <unordered_set>
#include <vector>

namespace rsmesh::isosurface {
    class mesh_defects_finder {
        using vertices_type = geometry::points3d;
        using face_type = Eigen::Matrix<index_t, 1, 3>;
        using faces_type = Eigen::Matrix<index_t, Eigen::Dynamic, 3, Eigen::RowMajor>;

    public:
        mesh_defects_finder(const vertices_type& vertices, const faces_type& faces);

        std::unordered_set<index_t> intersecting_faces() const;

        std::unordered_set<index_t> singular_vertices() const;

    private:
        bool segment_triangle_intersect(index_t vi, index_t vj, index_t fi) const;

        index_t next_vertex(index_t fi, index_t vi) const;

        index_t prev_vertex(index_t fi, index_t vi) const;

        const vertices_type& vertices_;
        const faces_type& faces_;
        std::vector<std::vector<index_t>> vf_map_;
    };
}

#endif //RSMESH_MESH_DEFECTS_FINDER_H
