//
// Created by RainSure on 2024/2/22.
//

#ifndef RSMESH_BIHARMONIC3D_H
#define RSMESH_BIHARMONIC3D_H

#include "rbf_base.h"

namespace rsmesh::rbf {
    class biharmonic3d final : public rbf_base {
    public:
        using rbf_base::rbf_base;

        explicit biharmonic3d(const std::vector<double>& params) { set_parameters(params); }

        [[nodiscard]] std::unique_ptr<rbf_base> clone() const override { return std::make_unique<biharmonic3d>(*this); }

        [[nodiscard]] int cpd_order() const override { return 1; }

        [[nodiscard]] double evaluate_isotropic(const vector3d& diff) const override {
            auto slope = parameters().at(0);
            auto r = diff.norm();

            return -slope * r;
        }

        [[nodiscard]] vector3d evaluate_gradient_isotropic(const vector3d& /*diff*/) const override {
            throw std::runtime_error("biharmonic3d::evaluate_gradient_isotropic() is not implemented");
        }

        [[nodiscard]] matrix3d evaluate_hessian_isotropic(const vector3d& /*diff*/) const override {
            throw std::runtime_error("biharmonic3d::evaluate_hessian_isotropic() is not implemented");
        }

        [[nodiscard]] int num_parameters() const override { return 1; }

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

#endif //RSMESH_BIHARMONIC3D_H
