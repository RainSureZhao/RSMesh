//
// Created by 赵润朔 on 2024/1/21.
//

#ifndef RSMESH_POLYNOMIAL_BASIC_BASE_H
#define RSMESH_POLYNOMIAL_BASIC_BASE_H

#include "common/macros.h"
#include "types.h"

namespace rsmesh::polynomial {
    class polynomial_basis_base {
    public:
        explicit polynomial_basis_base(int dimension, int degree) : dimension_(dimension), degree_(degree) {
            RSMESH_ASSERT(dimension >= 1 and dimension <= 3);
            RSMESH_ASSERT(degree >= 0);
        }

        virtual ~polynomial_basis_base() = default;
        polynomial_basis_base(const polynomial_basis_base&) = delete;
        polynomial_basis_base(polynomial_basis_base&&) = delete;

        polynomial_basis_base& operator=(const polynomial_basis_base&) = delete;
        polynomial_basis_base& operator=(polynomial_basis_base&&) = delete;

        [[nodiscard]] int dimension() const {return dimension_;}
        [[nodiscard]] int degree() const {return degree_;}

        index_t basis_size() const {return basis_size(dimension_, degree_);}

        static index_t basis_size(int dimension, int degree) {
            if(degree < 0) {
                return 0;
            }

            RSMESH_ASSERT(dimension >= 1 and dimension <= 3);
            auto k = index_t{degree} + 1;
            switch(dimension) {
                case 1:
                    return k;
                case 2:
                    return k * (k + 1) / 2;
                case 3:
                    return k * (k + 1) * (k + 2) / 6;
                default:
                    RSMESH_UNREACHABLE();
                    break;
            }

            return 0;
        }

    private:
        const int dimension_;
        const int degree_;
    };
}

#endif //RSMESH_POLYNOMIAL_BASIC_BASE_H
