//
// Created by 赵润朔 on 2024/2/12.
//

#ifndef RSMESH_RBF_EVALUATOR_H
#define RSMESH_RBF_EVALUATOR_H

#include "Eigen/Core"
#include <memory>
#include "common/macros.h"
#include "fmm/fmm_evaluator.h"
#include "fmm/fmm_tree_height.h"
#include "geometry/bbox3d.h"
#include "geometry/point3d.h"
#include "model.h"
#include "polynomial/monomial_basis.h"
#include "polynomial/polynomial_evaluator.h"
#include "types.h"

namespace rsmesh::interpolation {
    template<int Order = 10>
    class rbf_evaluator {
        using PolynomialEvaluator = polynomial::polynomial_evaluator<polynomial::monomial_basis>;
    public:
        rbf_evaluator(const model& model, const geometry::points3d& src_points) :
            n_poly_basis_(model.poly_basis_size()) {
            auto n_src_points = src_points.size();
            auto bbox = geometry::bbox3d::from_points(src_points);
            a_ = std::make_unique<fmm::fmm_evaluator<Order>>(model, fmm::fmm_tree_height(n_src_points), bbox);
            if(n_poly_basis_ > 0) {
                p_ = std::make_unique<PolynomialEvaluator>(model.poly_dimension(), model.poly_degree());
            }
            set_source_points(src_points);
        }

        rbf_evaluator(const model& model, const geometry::points3d& src_points, const geometry::bbox3d& bbox)
            : n_poly_basis_(model.poly_basis_size()){
            auto n_src_points = src_points.size();
            a_ = std::make_unique<fmm::fmm_evaluator<Order>>(model, fmm::fmm_tree_height(n_src_points), bbox);
            if(n_poly_basis_ > 0) {
                p_ = std::make_unique<PolynomialEvaluator>(model.poly_dimension(), model.poly_degree());
            }
            set_source_points(src_points);
        }

        rbf_evaluator(const model& model, int tree_height, const geometry::bbox3d& bbox)
            : n_poly_basis_(model.poly_basis_size()){
            a_ = std::make_unique<fmm::fmm_evaluator<Order>>(model, tree_height, bbox);
            if(n_poly_basis_ > 0) {
                p_ = std::make_unique<PolynomialEvaluator>(model.poly_dimension(), model.poly_degree());
            }
        }

        [[nodiscard]] valuesd evaluate() const {
            auto y = a_->evaluate();

            if(n_poly_basis_ > 0) {
                // add polynomial terms.
                y += p_->evaluate();
            }
            return y;
        }

        [[nodiscard]] valuesd evaluate(const geometry::points3d& points) const {
            set_field_points(points);
            return evaluate();
        }

        void set_field_points(const geometry::points3d& points) const {
            a_->set_field_points(points);
            if(n_poly_basis_ > 0) {
                p_->set_field_points(points);
            }
        }

        void set_source_points(const geometry::points3d& points) {
            n_src_points_ = points.rows();

            a_->set_source_points(points);
        }

        template<class Derived>
        void set_weights(const Eigen::MatrixBase<Derived>& weights) {
            RSMESH_ASSERT(weights.rows() == n_src_points_ + n_poly_basis_);

            a_->set_weights(weights.head(n_src_points_));

            if(n_poly_basis_ > 0) {
                p_->set_weights(weights.tail(n_poly_basis_));
            }
        }

    private:
        const index_t n_poly_basis_;

        index_t n_src_points_{};
        std::unique_ptr<fmm::fmm_evaluator<Order>> a_;
        std::unique_ptr<PolynomialEvaluator> p_;
    };
}

#endif //RSMESH_RBF_EVALUATOR_H
