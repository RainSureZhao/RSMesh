//
// Created by 赵润朔 on 2024/2/18.
//

#ifndef RSMESH_DOMAIN_H
#define RSMESH_DOMAIN_H

#include "geometry/bbox3d.h"
#include "types.h"
#include <vector>

namespace rsmesh::preconditioner {
    class domain_divider;
    class domain {
    public:
        std::vector<index_t> point_indices;
        std::vector<index_t> grad_point_indices;
        std::vector<bool> inner_point;
        std::vector<bool> inner_grad_point;

        index_t size() const;
        index_t grad_size() const;
        index_t mixed_size() const;

    private:
        friend class domain_divider;

        void merge_poly_points(const std::vector<index_t>& poly_point_idcs);

        geometry::bbox3d bbox_;
    };
} // namespace rsmesh::preconditioner

#endif //RSMESH_DOMAIN_H
