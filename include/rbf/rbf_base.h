//
// Created by RainSure on 2024/1/31.
//

#ifndef RSMESH_RBF_BASE_H
#define RSMESH_RBF_BASE_H

#include<memory>
#include"geometry/point3d.h"
#include<stdexcept>
#include<string>
#include<vector>

namespace rsmesh::rbf{
    class rbf_base {
    protected:
        using matrix3d = geometry::matrix3d;
        using vector3d = geometry::vector3d;

    public:
        virtual ~rbf_base() = default;

        rbf_base(rbf_base&&) = delete;
        rbf_base& operator=(const rbf_base&) = delete;
        rbf_base& operator=(rbf_base&&) = delete;

        [[nodiscard]] const geometry::linear_transformation3d& anisotropy() const {
            return aniso_;
        }

        [[nodiscard]] virtual std::unique_ptr<rbf_base> clone() const = 0;

        [[nodiscard]] virtual int cpd_order() const = 0;

        [[nodiscard]] double evaluate(const vector3d& diff) const {
            auto a_diff = geometry::transform_vector(aniso_, diff);
            return evaluate_isotropic(a_diff);
        }

        [[nodiscard]] vector3d evaluate_gradient(const vector3d& diff) const {
            auto a_diff = geometry::transform_vector(aniso_, diff);
            return evaluate_gradient_isotropic(a_diff);
        }

        [[nodiscard]] matrix3d evaluate_hessian(const vector3d& diff) const {
            auto a_diff = geometry::transform_vector(aniso_, diff);
            return aniso_.transpose() * evaluate_hessian_isotropic(a_diff) * aniso_;
        }

        [[nodiscard]] virtual double evaluate_isotropic(const vector3d& diff) const = 0;

        [[nodiscard]] virtual vector3d evaluate_gradient_isotropic(const vector3d& diff) const = 0;

        [[nodiscard]] virtual matrix3d evaluate_hessian_isotropic(const vector3d& diff) const = 0;

        [[nodiscard]] virtual int num_parameters() const = 0;

        [[nodiscard]] virtual const std::vector<double>& parameter_lower_bounds() const = 0;

        [[nodiscard]] virtual const std::vector<double>& parameter_upper_bounds() const = 0;

        [[nodiscard]] const std::vector<double>& parameters() const {return params_;}

        void set_anisotropy(const geometry::linear_transformation3d& aniso) {
            if(aniso.determinant() <= 0.0) {
                throw std::invalid_argument("aniso must have a position determinant");
            }

            aniso_ = aniso;
        }

        void set_parameters(const std::vector<double>& params) {
            if(static_cast<int>(params.size()) != num_parameters()) {
                throw std::invalid_argument("params.size() must have " + std::to_string(num_parameters()) + " elements.");
            }
            params_ = params;
        }
    protected:
        rbf_base() : aniso_(geometry::linear_transformation3d::Identity()) {}

        rbf_base(const rbf_base&) = default;

    private:
        std::vector<double> params_;
        geometry::linear_transformation3d aniso_;
    };
} // rsmesh::rbf

#endif //RSMESH_RBF_BASE_H
