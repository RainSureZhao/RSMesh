//
// Created by RainSure on 2024/2/13.
//

#ifndef RSMESH_GMRES_H
#define RSMESH_GMRES_H

#include "gmres_base.h"
#include "types.h"

namespace rsmesh::krylov {
    class gmres : public gmres_base {
    public:
        gmres(const linear_operator& op, const valuesd& rhs, index_t max_iter);

        void iterate_process() override;
    };
}

#endif //RSMESH_GMRES_H
