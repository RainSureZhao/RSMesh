//
// Created by 赵润朔 on 2024/2/13.
//

#ifndef RSMESH_INDENTITY_OPERATOR_H
#define RSMESH_INDENTITY_OPERATOR_H

#include "common/macros.h"
#include "linear_operator.h"
#include "types.h"

namespace rsmesh::krylov {
    class identity_operator : public linear_operator {
    public:
        explicit identity_operator(index_t n) : n_(n) {}

        valuesd operator()(const valuesd& v) const override {
            RSMESH_ASSERT(static_cast<int>(v.rows()) == n_);
            return v;
        }

        [[nodiscard]] index_t size() const override { return n_; }

    private:
        const index_t n_;
    };
}

#endif //RSMESH_INDENTITY_OPERATOR_H
