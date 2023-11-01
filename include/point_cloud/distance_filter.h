//
// Created by 赵润朔 on 2023/11/1.
//

#ifndef RSMESH_DISTANCE_FILTER_H
#define RSMESH_DISTANCE_FILTER_H

#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

#include "Eigen/Core"
#include "common/eigen_utility.h"
#include "geometry/point3d.h"
#include "types.h"

namespace rsmesh {
    namespace point_cloud {

        class distance_filter {
        public:
            distance_filter(const geometry::points3d& points, double distance_threshold);
            
            template<class Derived>
            auto filtered(const Eigen::MatrixBase<Derived>& m) const {
                if(m.rows() != n_points_) {
                    throw std::invalid_argument("The number of rows of the input matrix must be equal to the number of points in the point cloud.");
                }   
                return common::take_rows(m, filterer_indices_);
            }
            
            template <class Derived, class ...Args>
            auto filtered(const Eigen::MatrixBase<Derived>& m, Args&&... args) {
                return std::make_tuple(filtered(m), filtered(std::forward<Args>(args))...);
            } 
            
        private:
            const index_t n_points_;
            
            std::vector<index_t> filterer_indices_;
        };

    } // rsmesh
} // point_cloud

#endif //RSMESH_DISTANCE_FILTER_H
