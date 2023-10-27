//
// Created by 赵润朔 on 2023/10/27.
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

        class kdtree {
        public:
            using indices_and_distaces = std::pair<std::vector<index_t>, std::vector<double>>;
            
            kdtree(const geometry::points3d& points, bool use_exact_search);
            
            ~kdtree();
            
            kdtree(const kdtree&) = delete;             // 禁用默认的拷贝构造函数
            kdtree(kdtree&&) = delete;                  // 禁用默认的移动构造函数
            kdtree& operator=(const kdtree&) = delete;  // 禁用默认的拷贝赋值函数
            kdtree& operator=(kdtree&&) = delete;       // 禁用默认的移动赋值函数
            
            [[nodiscard]] indices_and_distaces knn_search(const geometry::point3d& point, index_t k) const;
            
            [[nodiscard]] indices_and_distaces radius_search(const geometry::point3d& point, double radius) const;
            
        private:
            class impl;                     // 前置声明, Pimpl设计模式
            std::unique_ptr<impl> impl_;
        };

    } // rsmesh
} // point_cloud

#endif //RSMESH_KDTREE_H
