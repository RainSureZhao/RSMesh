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
     * @namespace rsmesh::point_cloud
     * @brief 该命名空间下主要定义了点云计算相关的类和函数
     */
    namespace point_cloud {

        /**
         * @brief 用于对点云进行过滤，使得点云中任意两个点之间的距离大于某个阈值。
         */
        class distance_filter {
        public:
            distance_filter(const geometry::points3d& points, double distance);
            
            distance_filter(const geometry::points3d& points, double distance, const std::vector<index_t>& indices);


            /**
             * @brief 根据过滤器内部维护的索引对输入矩阵进行过滤。
             *
             * 这个操作符重载允许直接使用过滤器对象对点云或其他相关数据进行过滤。它返回输入矩阵的一个子集，
             * 该子集只包含过滤器内部索引指定的行。
             *
             * @tparam Derived 矩阵的类型，继承自Eigen的MatrixBase。
             * @param m 输入的Eigen矩阵，通常是点云数据。
             * @return 返回一个过滤后的矩阵，只包含被选中的行。
             *
             * @throws std::invalid_argument 如果输入矩阵的行数与点云中的点数不匹配。
             */
            template <class Derived>
            auto operator() (const Eigen::MatrixBase<Derived>& m) {
                if(m.rows() != n_points_) {
                    throw std::invalid_argument("The number of rows of the input matrix must be equal to the number of points in the point cloud.");
                }
                // Use .eval() to prevent memory corruption caused if the result is being assigned
                // back to the input matrix.
                return m(filtered_indices_, Eigen::all).eval();
            }

            /**
             * @brief 对多个相关矩阵应用相同的过滤规则并返回。
             *
             * 这个操作符重载允许同时对多个矩阵应用过滤规则，例如，当你有多个属性矩阵（颜色，法线等）需要
             * 根据相同的点云过滤索引进行过滤时。它使用单参数版本的`operator()`来对每个矩阵进行过滤，
             * 并将结果打包成一个std::tuple返回。
             *
             * @tparam Derived 矩阵的类型，继承自Eigen的MatrixBase。
             * @tparam Args 其他矩阵类型的变量模板参数列表。
             * @param m 输入的Eigen矩阵，通常是点云数据的主要矩阵。
             * @param args 输入的Eigen矩阵，其他相关数据矩阵。
             * @return 返回一个std::tuple，包含所有输入矩阵按过滤规则处理后的结果。
             */
            template <class Derived, class... Args>
            auto operator() (const Eigen::MatrixBase<Derived>& m, Args&&... args) {
                return std::make_tuple(operator()(m), operator()(std::forward<Args>(args))...);
            }

            /**
             * @brief 获取过滤后保留下来的点的索引列表。
             *
             * 这个函数返回一个包含所有通过距离过滤器规则保留下来的点的索引的列表。这些索引对应于
             * 最初传入构造函数的点云数据中的点。
             *
             * @return 保留下来的点的索引列表。
             */
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
