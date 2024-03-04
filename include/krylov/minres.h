//
// Created by RainSure on 2024/2/13.
//

#ifndef RSMESH_MINRES_H
#define RSMESH_MINRES_H

#include "gmres_base.h"
#include "types.h"

namespace rsmesh::krylov {
    class minres : public gmres_base {
    public:
        minres(const linear_operator& op, const valuesd& rhs, index_t max_iter);

        void iterate_process() override;

    private:
        double beta_{};
    };
} // namespace rsmesh::krylov

#endif //RSMESH_MINRES_H
