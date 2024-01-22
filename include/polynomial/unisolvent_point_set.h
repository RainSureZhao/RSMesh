//
// Created by 赵润朔 on 2024/1/22.
//

#ifndef RSMESH_UNISOLVENT_POINT_SET_H
#define RSMESH_UNISOLVENT_POINT_SET_H

#include "geometry/point3d.h"
#include "polynomial/lagrange_basis.h"
#include "polynomial/polynomial_basic_base.h"
#include "types.h"
#include <random>
#include <set>
#include <stdexcept>
#include <vector>

namespace rsmesh::polynomial {
    class unisolvent_point_set {
        static constexpr int kMaxTrial = 32;
    public:
        unisolvent_point_set(const geometry::vectors3d &points, int dimension, int degree) {
            if(degree < 0) return;
            auto n_points = points.rows();
            auto n_poly_basis = polynomial_basis_base::basis_size(dimension, degree);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<index_t> dist(index_t{0}, n_points - 1);

            std::set<index_t> set;
            auto found = false;
            auto trial = 0;
            while(!found and trial < kMaxTrial) {
                set.clear();

                while(static_cast<index_t>(set.size()) < n_poly_basis) {
                    set.insert(dist(gen));
                }

                try {
                    lagrange_basis basis(dimension, degree,
                                         points(std::vector<index_t>(begin(set), end(set)), Eigen::all));
                    found = true;
                } catch (const std::domain_error&) {
                    // do nothing
                }

                trial ++;
            }

            if(!found and trial == kMaxTrial) {
                throw std::runtime_error("Could not find a unisolvent set of points.");
            }

            point_idcs_.insert(begin(point_idcs_), begin(set), end(set));
        }

        [[nodiscard]] const std::vector<index_t>& point_indices() const {
            return point_idcs_;
        }

    private:
        std::vector<index_t> point_idcs_;
    };
}


#endif //RSMESH_UNISOLVENT_POINT_SET_H
