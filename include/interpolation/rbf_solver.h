//
// Created by 赵润朔 on 2024/2/4.
//

#ifndef RSMESH_RBF_SOLVER_H
#define RSMESH_RBF_SOLVER_H

#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include "Eigen/Core"
#include "common/macros.h"
#include "common/orthonormalize.h"
#include "geometry/bbox3d.h"
#include "geometry/point3d.h"
#include "interpolation/rbf_operator.h"
#include "interpolation/rbf_residual_evaluator.h"
#include "krylov/fgmres.h"
#include "model.h"
#include "polynomial/monomial_basis.h"
#include "preconditioner/ras_preconditioner.h"
#include "types.h"

namespace rsmesh::interpolation {
    class rbf_solver {
        using Preconditioner = preconditioner::ras_preconditioner;

    public:
        rbf_solver(const model& model, const geometry::points3d& points)
            : model_(model), n_poly_basis_(model.poly_basis_size()), n_points_(points.rows()) {
            op_ = std::make_unique<rbf_operator<>>(model, points);
            res_eval_ = std::make_unique<rbf_residual_evaluator>(model, points);

            set_points(points);
        }

        rbf_solver(const model& model, int tree_height, const geometry::bbox3d& bbox) :
            model_(model), n_poly_basis_(model.poly_basis_size()) {
            op_ = std::make_unique<rbf_operator<>>(model, tree_height, bbox);
            res_eval_ = std::make_unique<rbf_residual_evaluator>(model, tree_height, bbox);
        }

        void set_points(const geometry::points3d& points) {
            n_points_ = points.rows();

            op_->set_points(points);
            res_eval_->set_points(points);

            pc_ = std::make_unique<Preconditioner>(model_, points);
            if (n_poly_basis_ > 0) {
                polynomial::monomial_basis poly(model_.poly_dimension(), model_.poly_degree());
                p_ = poly.evaluate(points).transpose();
                common::orthonormalize_cols(p_);
            }
        }
        template <class Derived>
        valuesd solve(const Eigen::MatrixBase<Derived>& values, double absolute_tolerance,
                              int max_iter) const {
            RSMESH_ASSERT(values.rows() == n_points_);

            return solve_impl(values, absolute_tolerance, max_iter);
        }

        template <class Derived, class Derived2>
        valuesd solve(const Eigen::MatrixBase<Derived>& values, double absolute_tolerance,
                              int max_iter, const Eigen::MatrixBase<Derived2>& initial_solution) const {
            RSMESH_ASSERT(values.rows() == n_points_);
            RSMESH_ASSERT(initial_solution.rows() == n_points_ + n_poly_basis_);

            valuesd ini_sol = initial_solution;

            if (n_poly_basis_ > 0) {
                // Orthogonalize weights against P.
                auto n_cols = p_.cols();
                for (index_t i = 0; i < n_cols; i++) {
                    ini_sol.head(n_points_) -= p_.col(i).dot(ini_sol.head(n_points_)) * p_.col(i);
                }
            }

            return solve_impl(values, absolute_tolerance, max_iter, &ini_sol);
        }

    private:
        template <class Derived, class Derived2 = valuesd>
        valuesd solve_impl(const Eigen::MatrixBase<Derived>& values, double absolute_tolerance,
                           int max_iter,
                           const Eigen::MatrixBase<Derived2> *initial_solution = nullptr) const {
            // The solver does not work when all values are zero
            if(values.isZero()) {
                return valuesd::Zero(n_points_ + n_poly_basis_);
            }

            valuesd rhs(n_points_ + n_poly_basis_);
            rhs.head(n_points_) = values;
            rhs.tail(n_poly_basis_) = valuesd::Zero(n_poly_basis_);

            krylov::fgmres solver(*op_, rhs, max_iter);
            if(initial_solution != nullptr) {
                solver.set_initial_solution(*initial_solution);
            }
            solver.set_right_preconditioner(*pc_);
            solver.setup();

            std::cout << std::setw(4) << "iter" << std::setw(16) << "rel_res" << std::endl
                      << std::setw(4) << solver.iteration_count() << std::setw(16) << std::scientific
                      << solver.relative_residual() << std::defaultfloat << std::endl;
            valuesd solution;
            while(true) {
                solver.iterate_process();
                solution = solver.solution_vector();
                std::cout << std::setw(4) << solver.iteration_count() << std::setw(16) << std::scientific
                          << solver.relative_residual() << std::defaultfloat << std::endl;

                auto convergence = res_eval_->converged(values, solution, absolute_tolerance);
                if (convergence.first) {
                    std::cout << "Achieved absolute residual: " << convergence.second << std::endl;
                    break;
                }
                if(solver.iteration_count() == solver.max_iterations()) {
                    throw std::runtime_error("Reached the maximum number of iterations.");
                }
            }
        }
        const model& model_;
        const index_t n_poly_basis_;

        index_t n_points_{};
        std::unique_ptr<rbf_operator<>> op_;
        std::unique_ptr<Preconditioner> pc_;
        std::unique_ptr<rbf_residual_evaluator> res_eval_;
        Eigen::MatrixXd p_;
    };
}


#endif //RSMESH_RBF_SOLVER_H
