//
// Created by RainSure on 2023/10/27.
//

#ifndef RSMESH_KDTREE_H
#define RSMESH_KDTREE_H

#include <vector>
#include <memory>
#include <utility>

#include "geometry/point3d.h"
#include "types.h"

namespace rsmesh {
    namespace point_cloud {
        /**
         * \brief 用于在点云中搜索最近邻点的kdtree，利用flann库来进行实现
         *
         * 该类主要支持两种操作，第一种是k近邻搜索，第二种是半径搜索。
         */
        class kdtree {
        public:
            using indices_and_distaces = std::pair<std::vector<index_t>, std::vector<double>>;

            /**
             * \brief 构造函数
             * \param points 点云
             * \param use_exact_search 是否使用精确搜索
             */
            kdtree(const geometry::points3d& points, bool use_exact_search);

            /**
             * \brief 析构函数
             */
            ~kdtree();
            
            kdtree(const kdtree&) = delete;             // 禁用默认的拷贝构造函数
            kdtree(kdtree&&) = delete;                  // 禁用默认的移动构造函数
            kdtree& operator=(const kdtree&) = delete;  // 禁用默认的拷贝赋值函数
            kdtree& operator=(kdtree&&) = delete;       // 禁用默认的移动赋值函数

            /**
             * \brief 搜索距离某个点最近的k个点
             * @param point 输入的点
             * @param k k值
             * @return
             */
            [[nodiscard]] indices_and_distaces knn_search(const geometry::point3d& point, index_t k) const;

            /**
             * \brief 搜索出距离某个点距离小于等于radius的全部点
             * @param point 输入的点
             * @param radius 半径
             * @return
             */
            [[nodiscard]] indices_and_distaces radius_search(const geometry::point3d& point, double radius) const;
            
        private:
            class impl;                     // 前置声明, Pimpl设计模式
            std::unique_ptr<impl> impl_;
        };

    } // rsmesh
} // point_cloud

#endif //RSMESH_KDTREE_H
