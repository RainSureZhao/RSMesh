//
// Created by RainSure on 2024/2/13.
//

#ifndef RSMESH_FGMRES_H
#define RSMESH_FGMRES_H

#include "gmres.h"
#include "types.h"
#include <stdexcept>
#include <vector>

namespace rsmesh::krylov {
    class fgmres : public gmres {
    public:
        fgmres(const linear_operator& op, const valuesd& rhs, index_t max_iter);

        void set_left_preconditioner(const linear_operator& /*left_preconditioner*/) override {
            throw std::runtime_error("set_left_preconditioner is not supported.");
        }

        valuesd solution_vector() const override;

    private:
        void add_preconditioned_krylov_basis(const valuesd& z) override;

        // zs[i] := right_preconditioned(vs[i - 1]).
        std::vector<valuesd> zs_;
    };
}

#endif //RSMESH_FGMRES_H
