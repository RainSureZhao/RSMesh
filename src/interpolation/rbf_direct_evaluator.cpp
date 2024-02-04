//
// Created by 赵润朔 on 2024/2/3.
//
#include"interpolation/rbf_direct_evaluator.h"
#include"numeric/sum_accumulator.h"

namespace rsmesh::interpolation {
    rbf_direct_evaluator::rbf_direct_evaluator(const model& model, const geometry::points3d& source_points
            , const geometry::points3d& source_grad_points)
            : model_(model), dim_(model.poly_dimension()), l_(model.poly_basis_size()),
            mu_(source_points.rows()), sigma_(source_grad_points.rows()), src_points_(source_points), src_grad_points_(source_grad_points) {
        if(l_ > 0) {
            p_ = std::make_unique<PolynomialEvaluator>(model.poly_dimension(), model.poly_degree());
        }
    }

    rbf_direct_evaluator::rbf_direct_evaluator(const rsmesh::model &model, const geometry::points3d &source_points) :
            rbf_direct_evaluator(model, source_points, geometry::points3d(0, 3)) {

    }

    valuesd rbf_direct_evaluator::evaluate() const {
        auto y_accm = std::vector<numeric::sum_accumulator<double>>(n_fld_points_);

        auto weights = weights_.head(mu_);
        auto grad_weights = weights_.segment(mu_, dim_ * sigma_).reshaped<Eigen::RowMajor>(sigma_, dim_);

        const auto& rbf = model_.rbf();
        for(index_t i = 0; i < n_fld_points_; i ++) {
            for(index_t j = 0; j < mu_; j ++) {
                y_accm.at(i) += weights[j] * rbf.evaluate(fld_points_.row(i) - src_points_.row(j));
            }
            for(index_t j = 0; j < sigma_; j ++) {
                y_accm.at(i) += grad_weights.row(j).dot(
                        -rbf.evaluate_gradient(fld_points_.row(i) - src_points_.row(j)).head(dim_)
                        );
            }
        }
        if(l_ > 0) {
            // Add polynomial terms
            auto poly_val = p_->evaluate();
            for(index_t i = 0; i < n_fld_points_; i ++) {
                y_accm.at(i) += poly_val(i);
            }
        }

        valuesd y(n_fld_points_);
        for(index_t i = 0; i < n_fld_points_; i ++) {
            y(i) = y_accm.at(i).get();
        }
        return y;
    }

    void rbf_direct_evaluator::set_field_points(const geometry::points3d &field_points) {
        n_fld_points_ = static_cast<index_t>(field_points.rows());
        fld_points_ = field_points;
        if(l_ > 0) {
            p_->set_field_points(fld_points_);
        }
    }
}