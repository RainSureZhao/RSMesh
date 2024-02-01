//
// Created by 赵润朔 on 2024/1/31.
//

#ifndef RSMESH_RBF_FITTER_H
#define RSMESH_RBF_FITTER_H

#include "geometry/point3d.h"
#include "model.h"
#include "types.h"

namespace rsmesh::interpolation {
    class rbf_fitter {
        rbf_fitter(const model& model, const geometry::points3d& points) : model_(model), points_(points);

        valuesd fit(const valuesd& values, double absolute_tolerance, int max_iter) const;

    private:
        const model& model_;
        const geometry::points3d& points_;
    };
}

#endif //RSMESH_RBF_FITTER_H
