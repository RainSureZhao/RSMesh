//
// Created by RainSure on 2024/1/21.
//

#ifndef RSMESH_LAGRANGE_BASIS_H
#define RSMESH_LAGRANGE_BASIS_H

#include "Eigen/Core"
#include "Eigen/LU"
#include "Eigen/SVD"
#include "common/macros.h"
#include "geometry/point3d.h"
#include "polynomial/monomial_basis.h"
#include "polynomial/polynomial_basic_base.h"
#include "types.h"
#include <stdexcept>

/**
 * \namespace rsmesh::polynomial
 * \brief 该命名空间下主要定义了多项式计算相关的类和函数
 */
namespace rsmesh::polynomial {
    class lagrange_basis : public polynomial_basis_base {
        static constexpr double kRcondThreshold = 1e-10; // 用于判断奇异矩阵的阈值

    public:
        template<class Derived>
        lagrange_basis(int dimension, int degree, const Eigen::MatrixBase<Derived>& points) :
                polynomial_basis_base(dimension, degree), mono_basis_(dimension, degree) {
            RSMESH_ASSERT(points.rows() == basis_size());

            Eigen::MatrixXd p = mono_basis_.evaluate(points).transpose();

            if(!is_invertible(p)) {
                throw std::domain_error("The set of points is not unisolvent.");
            }

            coeffs_ = p.fullPivLu().inverse();
        }

        template<class Derived>
        Eigen::MatrixXd evaluate(const Eigen::MatrixBase<Derived>& points) const {
            return evaluate(points, geometry::points3d(0, 3));
        }

        template<class DerivedPoints, class DerivedGradPoints>
        Eigen::MatrixXd evaluate(const Eigen::MatrixBase<DerivedGradPoints> &points,
                                 const Eigen::MatrixBase<DerivedPoints> &grad_points) const {
            auto pt = mono_basis_.evaluate(points, grad_points);

            return coeffs_.transpose() * pt;
        }

    private:
        static bool is_invertible(const Eigen::MatrixXd& m) {
            auto svd = m.jacobiSvd();
            const auto& sigmas = svd.singularValues();
            if(sigmas(0) == 0.0) {
                return false;
            }

            auto rcond = sigmas(sigmas.rows() - 1) / sigmas(0);
            return rcond >= kRcondThreshold;
        }
        monomial_basis mono_basis_;

        Eigen::MatrixXd coeffs_;
    };
}// namespace rsmesh::polynomial

#endif //RSMESH_LAGRANGE_BASIS_H
