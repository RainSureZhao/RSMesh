//
// Created by RainSure on 2024/2/14.
//

#ifndef RSMESH_RBF_SYMMETRIC_EVALUATOR_H
#define RSMESH_RBF_SYMMETRIC_EVALUATOR_H

#include "Eigen/Core"
#include "common/macros.h"
#include "fmm/fmm_symmetric_evaluator.h"
#include "fmm/fmm_tree_height.h"
#include "geometry/bbox3d.h"
#include "geometry/point3d.h"
#include "model.h"
#include "polynomial/monomial_basis.h"
#include "polynomial/polynomial_evaluator.h"
#include "types.h"

namespace rsmesh::interpolation {
    template <int Order = 10>
    class rbf_symmetric_evaluator {
        using PolynomialEvaluator = polynomial::polynomial_evaluator<polynomial::monomial_basis>;

    public:
        rbf_symmetric_evaluator(const model& model, const geometry::points3d& points)
                : n_points_(points.rows()), n_poly_basis_(model.poly_basis_size()) {
            auto bbox = geometry::bbox3d::from_points(points);
            a_ = std::make_unique<fmm::fmm_symmetric_evaluator<Order>>(
                    model, fmm::fmm_tree_height(n_points_), bbox);
            a_->set_points(points);

            if (n_poly_basis_ > 0) {
                p_ = std::make_unique<PolynomialEvaluator>(model.poly_dimension(), model.poly_degree());
                p_->set_field_points(points);
            }
        }

        [[nodiscard]] valuesd evaluate() const {
            auto y = a_->evaluate();

            if (n_poly_basis_ > 0) {
                // Add polynomial terms.
                y += p_->evaluate();
            }

            return y;
        }

        template <class Derived>
        void set_weights(const Eigen::MatrixBase<Derived>& weights) {
            RSMESH_ASSERT(weights.rows() == n_points_ + n_poly_basis_);

            a_->set_weights(weights.head(n_points_));

            if (n_poly_basis_ > 0) {
                p_->set_weights(weights.tail(n_poly_basis_));
            }
        }

    private:
        const index_t n_points_;
        const index_t n_poly_basis_;

        std::unique_ptr<fmm::fmm_symmetric_evaluator<Order>> a_;
        std::unique_ptr<PolynomialEvaluator> p_;
    };
} // namespace rsmesh::interpolation

#endif //RSMESH_RBF_SYMMETRIC_EVALUATOR_H
