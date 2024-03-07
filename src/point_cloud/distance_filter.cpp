//
// Created by RainSure on 2023/11/1.
//

#include "point_cloud/distance_filter.h"
#include "point_cloud/kdtree.h"
#include <set>
#include <tuple>

namespace rsmesh {
    namespace point_cloud {

        /**
         * @brief 构造一个距离过滤器，使用所有点作为候选。
         *
         * 这个构造函数接受一个点集和一个距离阈值，然后使用所有点作为候选点来初始化过滤器。
         * 它通过调用第二个构造函数实现，传递所有点的索引作为候选。
         *
         * @param points 三维点的集合，每一行代表一个点的坐标。
         * @param distance 用于筛选邻近点的距离阈值。所有距离小于或等于此阈值的点将被认为是相邻的，
         *                 并可能从结果中排除，以确保没有点的邻域内有其他点。
         */
        distance_filter::distance_filter(const geometry::points3d &points, double distance) :
                distance_filter(points, distance, trivial_indices(static_cast<index_t >(points.rows()))) {
            
        }

        /**
         * @brief 构造一个距离过滤器，使用指定的点索引作为候选。
         *
         * 这个构造函数接受一个点集、一个距离阈值和一个点索引的集合。它使用这些索引指定的点作为候选，
         * 然后基于距离阈值来决定哪些点应该被保留。任何与另一个点的距离小于或等于指定距离的点都将被过滤掉，
         * 除非它是给定索引中的第一个邻近点。
         *
         * @param points 三维点的集合，每一行代表一个点的坐标。
         * @param distance 用于筛选邻近点的距离阈值。
         * @param indices 点的索引集合，指定哪些点应该考虑进行过滤。这允许对点集的子集进行过滤操作。
         *
         * @throws std::invalid_argument 如果distance阈值不大于0，则抛出异常。
         */
        distance_filter::distance_filter(const geometry::points3d &points, double distance,
                                         const std::vector<index_t> &indices) : n_points_(static_cast<index_t >(points.rows())) {
            if (distance <= 0.0) {
                throw std::invalid_argument("distance_threshold must be greater than 0.0");
            }

            kdtree tree(points, true);

            std::vector<index_t> nn_indices;
            std::vector<double> nn_distances;
            std::set<index_t> indices_to_remove;

            for (auto i : indices) {
                if(indices_to_remove.contains(i)) continue;

                geometry::points3d p = points.row(i);
                std::tie(nn_indices, nn_distances) = tree.radius_search(p, distance);

                for(auto j : nn_indices) {
                    if(j != i) {
                        indices_to_remove.insert(j);
                    }
                }
            }

            for(auto i : indices) {
                if(!indices_to_remove.contains(i)) {
                    filtered_indices_.push_back(i);
                }
            }
        }
    } // rsmesh
} // point_cloud