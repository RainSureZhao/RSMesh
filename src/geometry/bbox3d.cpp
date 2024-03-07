//
// Created by RainSure on 2023/10/26.
//

#include "geometry/bbox3d.h"
#include "common/eigen_utility.h"
#include <limits>

namespace rsmesh {
    namespace geometry {
        /**
         * @brief bbox3d类的默认构造函数
         *
         * 主要实现逻辑是将min_初始化为无穷大，max_初始化为负无穷大
         **/
        bbox3d::bbox3d() : min_(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()), 
            max_(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()){}

        /**
         * @brief bbox3d类的构造函数，根据传入的左下角的点和右上角的点来构造包围盒
         * @param min 传入的包围盒的左下角点
         * @param max 传入的包围盒的右上角点
         */
        bbox3d::bbox3d(const point3d &min, const point3d &max) : min_(min), max_(max){
        }
        /**
         * @brief 重载了==运算符，用于判断两个bbox3d是否相等
         * @param rhs 比较的另一个bbox3d
         * @return
         */
        bool bbox3d::operator==(const bbox3d &rhs) const {
            return min_ == rhs.min_ and max_ == rhs.max_;   
        }

        /**
         * @brief 返回包围盒的中心点
         * @return
         */
        point3d bbox3d::center() const {
            return min_ + size() / 2.0;
        }

        /**
         * @brief 判断包围盒是否包含某个点
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
         * @brief 计算该包围盒和另一个包围盒的凸包
         * @param other
         * @return
         */
        bbox3d bbox3d::convex_hull(const bbox3d& other) const {
            return {min_.cwiseMin(other.min_), max_.cwiseMax(other.max_)};
        }

        /**
         * @brief 返回包围盒的最大点
         * @return 返回包围盒的最大点, 类型为point3d
         */
        const point3d &bbox3d::max() const {
            return max_;
        }

        /**
         * @brief 返回包围盒的最小点
         * @return  返回包围盒的最小点, 类型为point3d
         */
        const point3d &bbox3d::min() const {
            return min_;
        }

        /**
         * @brief 返回包围盒的大小
         * @return 返回包围盒的大小, 类型为vector3d
         */
        vector3d bbox3d::size() const {
            return max_ - min_;
        }

        /**
         * @brief 对包围盒应用线性变换并返回变换后的包围盒。
         *
         * 该函数计算当前包围盒在给定线性变换下的新位置和大小。操作通过首先计算包围盒中心的变换，
         * 然后对包围盒的三个相对于中心的主向量进行变换，来实现。最后，基于变换后的向量重新计算
         * 包围盒的最小和最大点。
         *
         * @param t 线性变换对象，包含了应用于包围盒的变换矩阵。
         * @return 返回变换后的包围盒，这是一个新的`bbox3d`实例，表示在变换`t`下原始包围盒的新位置和范围。
         *
         * @note 变换包括平移、旋转和缩放等线性变换。该方法不改变原始包围盒，而是返回一个新的包围盒实例。
         */
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
        /**
         * @brief 计算并返回当前包围盒与另一个包围盒的并集包围盒。
         *
         * 并集包围盒是指一个新的包围盒，它包含了当前包围盒和另一个给定包围盒中所有点的最小集合。
         * 这意味着新的包围盒的每个维度的最小值是两个包围盒对应维度最小值的最小值，每个维度的最大值是
         * 两个包围盒对应维度最大值的最大值。
         *
         * @param other 参与计算并集的另一个包围盒。
         * @return 返回一个新的`bbox3d`实例，代表当前包围盒和参数`other`包围盒的并集。
         *
         * @note 这个函数不修改当前包围盒或参数包围盒的状态，而是返回一个全新的包围盒实例。
         */
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