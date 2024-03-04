//
// Created by RainSure on 2024/2/3.
//
#include "interpolation/rbf_fitter.h"
#include "interpolation/rbf_solver.h"
namespace rsmesh::interpolation {
    rbf_fitter::rbf_fitter(const model& model, const geometry::points3d& points) : model_(model), points_(points) {};

    valuesd rbf_fitter::fit(const rsmesh::valuesd &values, double absolute_tolerance, int max_iter) const {
        rbf_solver solver(model_, points_);
        return solver.solve(values, absolute_tolerance, max_iter);
    }
}