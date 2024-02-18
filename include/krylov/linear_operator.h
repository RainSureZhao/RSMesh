//
// Created by 赵润朔 on 2024/2/13.
//

#ifndef RSMESH_LINEAR_OPERATOR_H
#define RSMESH_LINEAR_OPERATOR_H

#include "types.h"

namespace rsmesh::krylov {
    class linear_operator {
    public:
        virtual ~linear_operator() = default;
        linear_operator(const linear_operator&) = delete;
        linear_operator(linear_operator&&) = delete;
        linear_operator& operator=(const linear_operator&) = delete;
        linear_operator& operator=(linear_operator&&) = delete;

        virtual valuesd operator()(const valuesd& x) const = 0;

        virtual index_t size() const = 0;
    protected:
        linear_operator() = default;
    };
}

#endif //RSMESH_LINEAR_OPERATOR_H
