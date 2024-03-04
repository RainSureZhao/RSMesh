//
// Created by RainSure on 2024/2/18.
//

#ifndef RSMESH_COARSE_GRID_H
#define RSMESH_COARSE_GRID_H

#include "Eigen/Cholesky"
#include "Eigen/Core"
#include "Eigen/LU"
#include "geometry/point3d.h"
#include "model.h"
#include "preconditioner/domain.h"
#include "types.h"
#include <vector>

/**
 * \namespace rsmesh::preconditioner
 * \brief 该命名空间下主要定义了关于krylov子空间方法的预处理相关的类和函数
 */
namespace rsmesh::preconditioner {
    class coarse_grid {
    public:
        coarse_grid(const model& model, domain&& domain);

        void clear();

        void setup(const geometry::points3d& points_full, const Eigen::MatrixXd& lagrange_pt_full);

        void setup(const geometry::points3d& points_full, const geometry::points3d& grad_points_full,
                   const Eigen::MatrixXd& lagrange_pt_full);

        void set_solution_to(Eigen::Ref<valuesd> weights_full) const;

        void solve(const Eigen::Ref<const valuesd>& values_full);
    private:
        const model& model_;
        const std::vector<index_t> point_idcs_;
        const std::vector<index_t> grad_point_idcs_;

        const index_t dim_;
        const index_t l_;
        const index_t mu_;
        const index_t sigma_;
        const index_t m_;
        index_t mu_full_;
        index_t sigma_full_;

        // Matrix -E.
        Eigen::MatrixXd me_;

        // Cholesky decomposition of matrix Q^T A Q.
        Eigen::LDLT<Eigen::MatrixXd> ldlt_of_qtaq_;

        // First l rows of matrix A.
        Eigen::MatrixXd a_top_;

        // LU decomposition of first l rows of matrix P.
        Eigen::FullPivLU<Eigen::MatrixXd> lu_of_p_top_;

        // Current solution.
        valuesd lambda_c_;
    };
}

#endif //RSMESH_COARSE_GRID_H
