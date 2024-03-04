//
// Created by RainSure on 2024/2/12.
//

#ifndef RSMESH_RBF_INCREMENTAL_FITTER_H
#define RSMESH_RBF_INCREMENTAL_FITTER_H

#include "geometry/bbox3d.h"
#include "geometry/point3d.h"
#include "model.h"
#include "types.h"
#include <utility>
#include <vector>

namespace rsmesh::interpolation {
    class rbf_incremental_fitter {
    public:
        rbf_incremental_fitter(const model& model, const geometry::points3d& points);

        std::pair<std::vector<index_t>, valuesd> fit(const valuesd& values, double absolute_tolerance, int max_iter) const;

    private:
        std::vector<index_t> complementary_indices(const std::vector<index_t>& indices) const;

        const model& model_;
        const geometry::points3d& points_;

        const index_t n_points_;
        const index_t n_poly_basis_;

        const geometry::bbox3d bbox_;
    };
}

#endif //RSMESH_RBF_INCREMENTAL_FITTER_H
