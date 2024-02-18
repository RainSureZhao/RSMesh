//
// Created by 赵润朔 on 2024/2/14.
//

#ifndef RSMESH_RBF_OPERATOR_H
#define RSMESH_RBF_OPERATOR_H

#include <memory>
#include "common/macros.h"
#include "fmm/fmm_symmetric_evaluator.h"
#include "fmm/fmm_tree_height.h"
#include "geometry/bbox3d.h"
#include "geometry/point3d.h"
#include "krylov/linear_operator.h"
#include "model.h"
#include "polynomial/monomial_basis.h"
#include "types.h"

namespace rsmesh::interpolation {
    template<int Order = 10>
    struct rbf_operator : krylov::linear_operator {
        rbf_operator(const model& model, const geometry::points3d& points) :
            model_(model), n_poly_basis_(model.poly_basis_size()) {
            auto n_points = points.rows();
            auto bbox = geometry::bbox3d::from_points(points);
            a_ = std::make_unique<fmm::fmm_symmetric_evaluator<Order>>(model, fmm::fmm_tree_height(n_points), bbox);

            if (n_poly_basis_ > 0) {
                poly_basis_ = std::make_unique<polynomial::monomial_basis>(model.poly_dimension(), model.poly_degree());
            }

            set_points(points);
        }

        rbf_operator(const model& model, int tree_height, const geometry::bbox3d& bbox)
            : model_(model), n_poly_basis_(model.poly_basis_size()),
            a_(std::make_unique<fmm::fmm_symmetric_evaluator<Order>>(model, tree_height, bbox)){
                if(n_poly_basis_ > 0) {
                    poly_basis_ = std::make_unique<polynomial::monomial_basis>(model.poly_dimension(), model.poly_degree());
                }
        }

        valuesd operator()(const valuesd& weights) const override {
            RSMESH_ASSERT(weights.rows() == size());

            valuesd y = valuesd::Zero(size());
            a_->set_weights(weights.head(n_points_));
            y.head(n_points_) = a_->evaluate();

            if(n_poly_basis_ > 0) {
                // Add polynomial terms
                y.head(n_points_) += pt_.transpose() * weights.tail(n_poly_basis_);
                y.tail(n_poly_basis_) += pt_ * weights.head(n_points_);
            }

            y.head(n_points_) += weights.head(n_points_) * model_.nugget();
            return y;
        }

        void set_points(const geometry::points3d& points) {
            n_points_ = points.rows();

            a_->set_points(points);
            if (n_poly_basis_ > 0) {
                pt_ = poly_basis_->evaluate(points);
            }
        }

        [[nodiscard]] index_t size() const override {
            return n_points_ + n_poly_basis_;
        }


    private:
        const model& model_;
        const index_t n_poly_basis_;

        index_t n_points_{};
        std::unique_ptr<fmm::fmm_symmetric_evaluator<Order>> a_;
        std::unique_ptr<polynomial::monomial_basis> poly_basis_;
        Eigen::MatrixXd  pt_;
    };
} // namespace rsmesh::interpolation

#endif //RSMESH_RBF_OPERATOR_H
