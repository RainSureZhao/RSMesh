//
// Created by 赵润朔 on 2024/2/13.
//

#ifndef RSMESH_RBF_INEQUALITY_FITTER_H
#define RSMESH_RBF_INEQUALITY_FITTER_H

#include "geometry/bbox3d.h"
#include "geometry/point3d.h"
#include "model.h"
#include "types.h"
#include <utility>
#include <vector>

namespace rsmesh::interpolation {
    class rbf_inequality_fitter {
    public:
        rbf_inequality_fitter(const model& model, const geometry::points3d& points);

        std::pair<std::vector<index_t >, valuesd> fit(const valuesd& values,
                                                      const valuesd& values_lb,
                                                      const valuesd& values_ub,
                                                      double absolute_tolerance,
                                                      int max_iter) const;

    private:
        template<class Predicate>
        static std::vector<index_t> arg_where(const valuesd& v, Predicate predicate) {
            std::vector<index_t> indices;
            for (index_t i = 0; i < v.size(); ++i) {
                if (predicate(v[i])) {
                    indices.push_back(i);
                }
            }
            return indices;
        }

        std::vector<index_t> complementary_indices(const std::vector<index_t>& indices) const;

        const model& model_;
        const geometry::points3d& points_;

        const index_t n_points_;
        const index_t n_poly_basis_;

        const geometry::bbox3d bbox_;
    };
} // namespace rsmesh::interpolation

#endif //RSMESH_RBF_INEQUALITY_FITTER_H
