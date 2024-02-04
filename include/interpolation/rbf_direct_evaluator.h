//
// Created by 赵润朔 on 2024/2/3.
//

#ifndef RSMESH_RBF_DIRECT_EVALUATOR_H
#define RSMESH_RBF_DIRECT_EVALUATOR_H

#include<memory>
#include"common/macros.h"
#include"geometry/point3d.h"
#include"model.h"
#include"polynomial/monomial_basis.h"
#include"polynomial/polynomial_evaluator.h"
#include"types.h"

namespace rsmesh::interpolation {
    class rbf_direct_evaluator {
        using PolynomialEvaluator = polynomial::polynomial_evaluator<polynomial::monomial_basis>;

    public:
        rbf_direct_evaluator(const model& model, const geometry::points3d& source_points);

        rbf_direct_evaluator(const model& model, const geometry::points3d& source_points,
                             const geometry::points3d& source_grad_points);

        [[nodiscard]] valuesd evaluate() const;

        void set_field_points(const geometry::points3d& field_points);
    private:
        const model& model_;
        const index_t dim_;
        const index_t l_;
        const index_t mu_;
        const index_t sigma_;
        const geometry::points3d src_points_;
        const geometry::points3d src_grad_points_;

        std::unique_ptr<PolynomialEvaluator> p_;

        index_t n_fld_points_;
        geometry::points3d fld_points_;
        valuesd weights_;
    };
}


#endif //RSMESH_RBF_DIRECT_EVALUATOR_H
