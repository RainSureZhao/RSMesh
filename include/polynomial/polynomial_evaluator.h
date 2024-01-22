//
// Created by 赵润朔 on 2024/1/21.
//

#ifndef RSMESH_POLYNOMIAL_EVALUATOR_H
#define RSMESH_POLYNOMIAL_EVALUATOR_H

#include "Eigen/Core"
#include "common/macros.h"
#include "geometry/point3d.h"
#include "types.h"

namespace rsmesh::polynomial {
    template <class Basis>
    class polynomial_evaluator {
    public:
        explicit polynomial_evaluator(int dimension, int degree) :
            basis_(dimension, degree), weights_(valuesd::Zero(basis_.basis_size())) {}
        [[nodiscard]] valuesd evaluate() const {
            Eigen::MatrixXd pt = basis_.evaluate(points_);
            return pt.transpose() * weights_;
        }

        void set_field_points(const geometry::points3d& points) {
            points_ = points;
        }

        void set_weights(const valuesd& weights) {
            RSMESH_ASSERT(weights.rows() == basis_.basis_size());
            weights_ = weights;
        }
    private:
        const Basis basis_;

        geometry::points3d points_;
        valuesd weights_;

    };
}// namespace rsmesh::polynomial


#endif //RSMESH_POLYNOMIAL_EVALUATOR_H
