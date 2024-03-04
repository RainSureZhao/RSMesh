//
// Created by RainSure on 2024/1/31.
//

#ifndef RSMESH_MODEL_H
#define RSMESH_MODEL_H

#include<limits>
#include<memory>
#include"polynomial/polynomial_basic_base.h"
#include"rbf/rbf_base.h"
#include"types.h"
#include<stdexcept>
#include<vector>

namespace rsmesh {
    /**
     * \class model
     * \brief 描述了一个插值模型
     */
    class model{
    public:
        model(const rbf::rbf_base& rbf, int poly_dimension, int poly_degree) : rbf_(rbf.clone()),
            poly_dimension_(poly_dimension), poly_degree_(poly_degree) {
            if(poly_dimension < 0 or poly_dimension > 3) {
                throw std::invalid_argument("poly_dimension must be within the range of 0 to 3.");
            }
            if(poly_degree < rbf.cpd_order() - 1 or poly_degree > 2) {
                throw std::invalid_argument("poly_degree must be within the range of rbf.cpd_order() - 1 to 2.");
            }
        }

        ~model() = default;

        model(const model& model) : rbf_(model.rbf_->clone()), poly_dimension_(model.poly_dimension_),
            poly_degree_(model.poly_degree_), nugget_(model.nugget_) {}

        model(model&& model) = default;

        model& operator=(const model&) = delete;
        model& operator=(model&&) = delete;

        [[nodiscard]] double nugget() const {return nugget_;}

        [[nodiscard]] int num_parameters() const {return 1 + rbf_->num_parameters();}

        [[nodiscard]] std::vector<double> parameter_lower_bounds() const {
            std::vector<double> lower_bounds{0.0};
            lower_bounds.insert(std::end(lower_bounds), std::begin(rbf_->parameter_lower_bounds()), std::end(rbf_->parameter_lower_bounds()));
            return lower_bounds;
        }

        [[nodiscard]] std::vector<double> parameter_upper_bounds() const {
            std::vector<double> upper_bounds{std::numeric_limits<double>::infinity()};
            upper_bounds.insert(std::end(upper_bounds), std::begin(rbf_->parameter_upper_bounds()), std::end(rbf_->parameter_upper_bounds()));
            return upper_bounds;
        }

        [[nodiscard]] std::vector<double> parameters() const {
            std::vector<double> params{nugget()};
            params.insert(std::end(params), std::begin(rbf_->parameters()), std::end(rbf_->parameters()));
            return params;
        }

        [[nodiscard]] index_t poly_basis_size() const {
            return polynomial::polynomial_basis_base::basis_size(poly_dimension_, poly_degree_);
        }

        [[nodiscard]] int poly_degree() const {
            return poly_degree_;
        }

        [[nodiscard]] int poly_dimension() const {
            return poly_dimension_;
        }

        [[nodiscard]] const rbf::rbf_base& rbf() const {return *rbf_;}

        void set_nugget(double nugget) {
            if(nugget < 0.0) {
                throw std::invalid_argument("nugget mest be greater than or equal to 0.0.");
            }

            nugget_ = nugget;
        }

        void set_parameters(const std::vector<double>& params) {
            if(static_cast<int>(params.size()) != num_parameters()) {
                throw std::invalid_argument("params.size() must be " + std::to_string(num_parameters()) + ".");
            }
            set_nugget(params[0]);
            rbf_->set_parameters(std::vector<double>(std::begin(params) + 1, std::end(params)));
        }

        [[nodiscard]] model without_poly() const {
            return model(*rbf_);
        }

    private:
        explicit model(const rbf::rbf_base& rbf) : rbf_(rbf.clone()), poly_dimension_(-1), poly_degree_(-1) {}

        std::unique_ptr<rbf::rbf_base> rbf_;
        int poly_dimension_;
        int poly_degree_;
        double nugget_{}; // 针对高斯过程的参数
    };
}

#endif //RSMESH_MODEL_H
