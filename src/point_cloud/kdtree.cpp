//
// Created by RainSure on 2023/10/27.
//

#include "point_cloud/kdtree.h"
#include "common/macros.h"
#include <cmath>
#include <stdexcept>

#include "flann/flann.hpp"

namespace rsmesh {
    namespace point_cloud {
        /**
         * @brief kdtree的内部实现类，封装了FLANN库以提供快速空间搜索功能。
         *
         * 此类是kdtree的实现细节，使用FLANN (Fast Library for Approximate Nearest Neighbors) 库
         * 来构建和查询k-d树，从而实现对点云数据的高效最近邻和半径搜索。
         */
        class kdtree::impl {
            using FlannIndex = flann::Index<flann::L2<double>>; ///< 使用欧氏距离度量的FLANN索引
        public:
            /**
             * @brief 定义搜索结果类型，包含索引和对应的距离。
             */
            using indices_and_distaces = std::pair<std::vector<index_t>, std::vector<double>>;

            /**
             * @brief 构造函数，基于点云数据构建k-d树。
             *
             * @param points 点云数据，每一行代表空间中的一个点。
             * @param use_exact_search 指定是否使用精确搜索。如果为true，搜索将没有性能优化的近似步骤。
             */
            impl(const geometry::points3d& points, bool use_exact_search) {

                flann::Matrix<double> points_mat(const_cast<double *>(points.data()), points.rows(), 3);
                
                flann_index_ = std::make_unique<FlannIndex>(points_mat, flann::KDTreeSingleIndexParams());
                flann_index_->buildIndex();
                
                if(use_exact_search) {
                    params_knn_.checks = flann::FLANN_CHECKS_UNLIMITED;
                    params_radius_.checks = flann::FLANN_CHECKS_UNLIMITED;
                }
            }

            /**
             * @brief 执行k最近邻搜索。
             *
             * @param point 搜索的目标点。
             * @param k 指定返回的最近邻点的数量。
             * @return 返回包含最近邻点索引和对应距离的pairs。
             */
            [[nodiscard]]indices_and_distaces knn_search(const geometry::point3d& point, index_t k) const {
                flann::Matrix<double> point_mat(const_cast<double *>(point.data()), 1, 3);
                std::vector<std::vector<size_t>> indices_v;
                std::vector<std::vector<double>> distaces_v;

                (void)flann_index_->knnSearch(point_mat, indices_v, distaces_v, k, params_knn_);
                
                std::vector<index_t> indices;
                indices.reserve(indices_v[0].size());
                
                for(auto i : indices_v[0]) {
                    indices.push_back(static_cast<index_t>(i));
                }
                
                for(auto &d : distaces_v[0]) {
                    d = std::sqrt(d);
                }
                
                RSMESH_ASSERT(indices.size() == distaces_v[0].size() and indices.size() == k);
                
                return {std::move(indices), std::move(distaces_v[0])};
            }

            /**
             * @brief 执行半径搜索。
             *
             * @param point 搜索的目标点。
             * @param radius 搜索半径，函数将返回所有在这个半径内的点。
             * @return 返回包含在指定半径内的点的索引和对应距离的pairs。
             */
            [[nodiscard]] indices_and_distaces radius_search(const geometry::points3d& point, double radius) const {
                flann::Matrix<double> point_mat(const_cast<double *>(point.data()), 1, 3);
                std::vector<std::vector<size_t>> indices_v;
                std::vector<std::vector<double>> distances_v;
                
                auto radius_sq = static_cast<float>(radius * radius);
                (void)flann_index_->radiusSearch(point_mat, indices_v, distances_v, radius_sq, params_radius_);
                
                std::vector<index_t> indices;
                indices.reserve(indices_v[0].size());
                
                for(auto i : indices_v[0]) {
                    indices.push_back(static_cast<index_t>(i));
                }
                
                for(auto &d : distances_v[0]) {
                    d = std::sqrt(d);
                }
                
                return {std::move(indices), std::move(distances_v[0])};
            }
            
        private:
            flann::SearchParams params_knn_; ///< KNN搜索参数
            flann::SearchParams params_radius_; ///< 半径搜索参数
            std::unique_ptr<FlannIndex> flann_index_; ///< FLANN索引的智能指针，用于执行空间搜索
        };
        
        kdtree::kdtree(const geometry::points3d &points, bool use_exact_search) : 
            impl_(points.rows() == 0 ? nullptr : std::make_unique<impl>(points, use_exact_search)) {}
            
        kdtree::~kdtree() = default;

        kdtree::indices_and_distaces kdtree::knn_search(const geometry::point3d &point, rsmesh::index_t k) const {
            if (k <= 0) {
                throw std::invalid_argument("k must be greater than 0");
            }
            if(impl_ == nullptr) {
                return {};
            }
            
            return impl_->knn_search(point, k);
        }

        kdtree::indices_and_distaces kdtree::radius_search(const geometry::point3d &point, double radius) const {
            if(radius <= 0.0) {
                throw std::invalid_argument("radius must be greater than 0.0");
            }
            
            if(impl_ == nullptr) {
                return {};
            }
            return impl_->radius_search(point, radius);
        }
    } // rsmesh
} // point_cloud