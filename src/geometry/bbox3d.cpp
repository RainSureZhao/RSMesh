//
// Created by RainSure on 2023/10/26.
//

#include "geometry/bbox3d.h"
#include "common/eigen_utility.h"
#include <limits>

namespace rsmesh {
    namespace geometry {
        /**
         * \brief bbox3d类的默认构造函数
         *
         * 主要实现逻辑是将min_初始化为无穷大，max_初始化为负无穷大
         */
        bbox3d::bbox3d() : min_(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()), 
            max_(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()){}

        /**
         * \brief bbox3d类的构造函数，根据传入的左下角的点和右上角的点来构造包围盒
         * @param min 传入的包围盒的左下角点
         * @param max 传入的包围盒的右上角点
         */
        bbox3d::bbox3d(const point3d &min, const point3d &max) : min_(min), max_(max){
        }
        /**
         * \brief 重载了==运算符，用于判断两个bbox3d是否相等
         * @param rhs 比较的另一个bbox3d
         * @return
         */
        bool bbox3d::operator==(const bbox3d &rhs) const {
            return min_ == rhs.min_ and max_ == rhs.max_;   
        }

        /**
         * \brief 返回包围盒的中心点
         * @return
         */
        point3d bbox3d::center() const {
            return min_ + size() / 2.0;
        }

        /**
         * 判断包围盒是否包含某个点
         * @param p 判断的点
         * @return
         */
        bool bbox3d::contains(const point3d &p) const {
            return 
            p[0] >= min_[0] and p[0] <= max_[0] and 
            p[1] >= min_[1] and p[1] <= max_[1] and 
            p[2] >= min_[2] and p[2] <= max_[2];
        }

        /**
         * 计算该包围盒和另一个包围盒的凸包
         * @param other
         * @return
         */
        bbox3d bbox3d::convex_hull(const bbox3d& other) const {
            return {min_.cwiseMin(other.min_), max_.cwiseMax(other.max_)};
        }

        const point3d &bbox3d::max() const {
            return max_;
        }

        const point3d &bbox3d::min() const {
            return min_;
        }

        vector3d bbox3d::size() const {
            return max_ - min_;
        }

        bbox3d bbox3d::transform(const linear_transformation3d &t) const {
            auto c = center();
            vector3d v1 = point3d(min_[0], max_[1], max_[2]) - c;
            vector3d v2 = point3d(max_[0], min_[1], max_[2]) - c;
            vector3d v3 = point3d(max_[0], max_[1], min_[2]) - c;
            
            c = transform_point(t, c);
            v1 = transform_vector(t, v1);
            v2 = transform_vector(t, v2);
            v3 = transform_vector(t, v3);
            
            vectors3d vertices(8, 3);
            vertices.row(0) = -v1 - v2 - v3; // min, min, min
            vertices.row(1) = -v1;           // max, min, min
            vertices.row(2) = v3;            // max, max, min
            vertices.row(3) = -v2;           // min, max, min
            vertices.row(4) = v1;            // min, max, max
            vertices.row(5) = -v3;           // min, min, max
            vertices.row(6) = v2;            // max, min, max
            vertices.row(7) = v1 + v2 + v3;  // max, max, max
            
            auto min = c + vertices.colwise().minCoeff();
            auto max = c + vertices.colwise().maxCoeff();
            
            return {min, max};
        }

        bbox3d bbox3d::union_hull(const bbox3d &other) const {
            return {
                    min().cwiseMin(other.min()),
                    max().cwiseMax(other.max())
            };
        }
        // 以下代码无用了，替换为了更简洁的模板写法
//        bbox3d bbox3d::from_points(const points3d &points) {
//            return from_points(common::row_begin(points), common::row_end(points));
//        }
        
    } // rsmesh
} // geometry