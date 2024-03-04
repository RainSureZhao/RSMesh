//
// Created by RainSure on 2024/2/13.
//

#ifndef RSMESH_GMRES_BASE_H
#define RSMESH_GMRES_BASE_H

#include "Eigen/Core"
#include "linear_operator.h"
#include "types.h"
#include <vector>

namespace rsmesh::krylov {
    class gmres_base {
    public:
        static bool print_progress;

        virtual ~gmres_base() = default;

        gmres_base(const gmres_base &) = delete;

        gmres_base(gmres_base &&) = delete;

        gmres_base &operator=(const gmres_base &) = delete;

        gmres_base &operator=(gmres_base &&) = delete;

        [[nodiscard]] double absolute_residual() const;

        [[nodiscard]] bool converged() const;

        virtual void iterate_process() = 0;

        [[nodiscard]] index_t iteration_count() const;

        [[nodiscard]] index_t max_iterations() const;

        [[nodiscard]] double relative_residual() const;

        virtual void set_left_preconditioner(const linear_operator &left_preconditioner);

        void set_initial_solution(const valuesd &x0);

        virtual void set_right_preconditioner(const linear_operator &right_preconditioner);

        virtual void setup();

        virtual valuesd solution_vector() const;

        // tolerance: Tolerance of the relative residual (stopping criterion).
        void solve(double tolerance);

    protected:
        gmres_base(const linear_operator &op, const valuesd &rhs, index_t max_iter);

        virtual void add_preconditioned_krylov_basis(const valuesd & /*z*/) {}

        [[nodiscard]] valuesd left_preconditioned(const valuesd &x) const;

        [[nodiscard]] valuesd right_preconditioned(const valuesd &x) const;

        const linear_operator &op_;

        // Dimension.
        const index_t m_;

        // Maximum # of iteration.
        const index_t max_iter_;

        // Initial solution.
        valuesd x0_;

        // Left preconditioner.
        const linear_operator *left_pc_{};

        // Right preconditioner.
        const linear_operator *right_pc_{};

        // Current # of iteration.
        index_t iter_{};

        // Constant (right-hand side) vector.
        const valuesd rhs_;

        // L2 norm of rhs.
        double rhs_norm_;

        // Orthonormal basis vectors for the Krylov subspace.
        std::vector<valuesd> vs_;

        // Upper triangular matrix of QR decomposition.
        Eigen::MatrixXd r_;

        // Cosines for the Givens rotations.
        valuesd c_;

        // Sines for the Givens rotations.
        valuesd s_;

        // Sequence of residuals.
        valuesd g_;

        bool converged_{};
    };
} // namespace rsmesh::krylov

#endif //RSMESH_GMRES_BASE_H
