//
// Created by RainSure on 2023/11/1.
//

#ifndef RSMESH_DISTANCE_FILTER_H
#define RSMESH_DISTANCE_FILTER_H

#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>
#include <numeric>

#include "Eigen/Core"
#include "common/eigen_utility.h"
#include "geometry/point3d.h"
#include "types.h"

namespace rsmesh {
    /**
     * \namespace rsmesh::point_cloud
     * \brief 该命名空间下主要定义了点云计算相关的类和函数
     */
    namespace point_cloud {

        class distance_filter {
        public:
            distance_filter(const geometry::points3d& points, double distance);
            
            distance_filter(const geometry::points3d& points, double distance, const std::vector<index_t>& indices);
            
            template<class Derived>
            auto filtered(const Eigen::MatrixBase<Derived>& m) const {
                if(m.rows() != n_points_) {
                    throw std::invalid_argument("The number of rows of the input matrix must be equal to the number of points in the point cloud.");
                }   
                return common::take_rows(m, filtered_indices_);
            }
            
            template <class Derived, class ...Args>
            auto filtered(const Eigen::MatrixBase<Derived>& m, Args&&... args) {
                return std::make_tuple(filtered(m), filtered(std::forward<Args>(args))...);
            } 
            
            template <class Derived>
            auto operator() (const Eigen::MatrixBase<Derived>& m) {
                if(m.rows() != n_points_) {
                    throw std::invalid_argument("The number of rows of the input matrix must be equal to the number of points in the point cloud.");
                }
                return m(filtered_indices_, Eigen::all).eval();
            }
            
            template <class Derived, class... Args>
            auto operator() (const Eigen::MatrixBase<Derived>& m, Args&&... args) {
                return std::make_tuple(operator()(m), operator()(std::forward<Args>(args))...);
            }
            
            [[nodiscard]] const std::vector<index_t>& filtered_indices() const {return filtered_indices_;}
        private:
            
            static std::vector<index_t> trivial_indices(index_t n_points) {
                std::vector<index_t> indices(n_points);
                std::iota(begin(indices), end(indices), 0);
                return indices;
            }
            
            const index_t n_points_;
            
            std::vector<index_t> filtered_indices_;
        };

    } // rsmesh
} // point_cloud

#endif //RSMESH_DISTANCE_FILTER_H
