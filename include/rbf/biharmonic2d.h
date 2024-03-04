//
// Created by RainSure on 2024/2/22.
//

#ifndef RSMESH_BIHARMONIC2D_H
#define RSMESH_BIHARMONIC2D_H

#include "rbf_base.h"

/**
 * \namespace rsmesh::rbf
 * \brief 该命名空间下主要定义了径向基函数相关的类和函数
 */
namespace rsmesh::rbf {
    class biharmonic2d final : public rbf_base {
    public:
        using rbf_base::rbf_base;

        explicit biharmonic2d(const std::vector<double>& params) {
            set_parameters(params);
        }

        [[nodiscard]] std::unique_ptr<rbf_base> clone() const override {
            return std::make_unique<biharmonic2d>(*this);
        }

        [[nodiscard]] int cpd_order() const override {
            return 2;
        }

        [[nodiscard]] double evaluate_isotropic(const vector3d& diff) const override {
            auto slope = parameters().at(0);
            auto r = diff.norm();

            if(r == 0.0) {
                return 0.0;
            }
            return slope * r * r * std::log(r);
        }

        [[nodiscard]] vector3d evaluate_gradient_isotropic(const vector3d& diff) const override {
            auto slope = parameters().at(0);
            auto r = diff.norm();

            if(r == 0.0) {
                return vector3d::Zero();
            }

            auto coeff = slope * (1.0 + 2.0 * std::log(r));
            return coeff * diff;
        }

        [[nodiscard]] matrix3d evaluate_hessian_isotropic(const vector3d& ) const override {
            throw std::runtime_error("Hessian of biharmonic2d is not implemented");
        }

        [[nodiscard]] int num_parameters() const override {
            return 1;
        }

        [[nodiscard]] const std::vector<double>& parameter_lower_bounds() const override {
            static const std::vector<double> lower_bounds{0.0};
            return lower_bounds;
        }

        [[nodiscard]] const std::vector<double>& parameter_upper_bounds() const override {
            static const std::vector<double> upper_bounds{std::numeric_limits<double>::infinity()};
            return upper_bounds;
        }
    };

} // namespace rsmesh::rbf

#endif //RSMESH_BIHARMONIC2D_H
