//
// Created by 赵润朔 on 2024/2/14.
//

#ifndef RSMESH_RBF_RESIDUAL_EVALUATOR_H
#define RSMESH_RBF_RESIDUAL_EVALUATOR_H

#include "Eigen/Core"
#include "common/macros.h"
#include "geometry/bbox3d.h"
#include "geometry/point3d.h"
#include "rbf_evaluator.h"
#include "model.h"
#include "types.h"
#include <cmath>
#include <algorithm>
#include <memory>
#include <utility>

namespace rsmesh::interpolation {
    class rbf_residual_evaluator {
        static constexpr index_t chunk_size = 1024;

    public:
        rbf_residual_evaluator(const model& model, const geometry::points3d& points)
                : model_(model),
                  n_poly_basis_(model.poly_basis_size()),
                  n_points_(points.rows()),
                  points_(points) {
            evaluator_ = std::make_unique<rbf_evaluator<>>(model, points_);
        }

        rbf_residual_evaluator(const model& model, int tree_height, const geometry::bbox3d& bbox)
                : model_(model), n_poly_basis_(model.poly_basis_size()) {
            evaluator_ = std::make_unique<rbf_evaluator<>>(model, tree_height, bbox);
        }

        template <class Derived, class Derived2>
        std::pair<bool, double> converged(const Eigen::MatrixBase<Derived>& values,
                                          const Eigen::MatrixBase<Derived2>& weights,
                                          double absolute_tolerance) const {
            RSMESH_ASSERT(values.rows() == n_points_);
            RSMESH_ASSERT(weights.rows() == n_points_ + n_poly_basis_);

            evaluator_->set_weights(weights);

            auto nugget = model_.nugget();

            auto max_residual = 0.0;
            for (index_t i = 0; i < n_points_ / chunk_size + 1; i++) {
                auto begin = i * chunk_size;
                auto end = std::min(n_points_, begin + chunk_size);
                if (begin == end) {
                    break;
                }

                evaluator_->set_field_points(points_.middleRows(begin, end - begin));
                valuesd fit = evaluator_->evaluate() + weights.segment(begin, end - begin) * nugget;

                for (index_t j = 0; j < end - begin; j++) {
                    auto res = std::abs(values(begin + j) - fit(j));
                    if (res >= absolute_tolerance) {
                        return {false, 0.0};
                    }

                    max_residual = std::max(max_residual, res);
                }
            }

            return {true, max_residual};
        }

        void set_points(const geometry::points3d& points) {
            n_points_ = points.rows();
            points_ = points;

            evaluator_->set_source_points(points);
        }

    private:
        const model& model_;
        const index_t n_poly_basis_;

        index_t n_points_{};
        geometry::points3d points_;

        std::unique_ptr<rbf_evaluator<>> evaluator_;
    };
}

#endif //RSMESH_RBF_RESIDUAL_EVALUATOR_H