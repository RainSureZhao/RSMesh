//
// Created by RainSure on 2023/10/25.
//

#ifndef RSMESH_CUBOID3D_H
#define RSMESH_CUBOID3D_H

#include "point3d.h"

namespace rsmesh {
    namespace geometry {
        /**
         * @brief 三维空间中的长方体表示。
         *
         * `cuboid3d`类提供了一个三维空间中的长方体（或立方体）的简单抽象。它通过两个点来定义，
         * 一个是长方体的最小顶点（在所有坐标轴上取最小值的顶点），另一个是最大顶点（在所有坐标轴上
         * 取最大值的顶点）。这种表示方法允许快速计算和简化与长方体相关的各种几何操作，如相交检测、
         * 体积计算等。
         *
         * @note 长方体的边界是闭合的，即包含其所有边界点。
         */
        class cuboid3d {
        public:
            /**
             * @brief 默认构造函数，创建一个单位长方体。
             *
             * 初始化一个原点处的最小点和一个在每个轴上长度为1的最大点的长方体。
             */
            cuboid3d() : min_(point3d::Zero()), max_(point3d::Ones()) { }

            /**
             * @brief 参数化构造函数，通过指定最小点和最大点来创建长方体。
             *
             * @param min 长方体的最小顶点。
             * @param max 长方体的最大顶点。
             */
            cuboid3d(point3d min, point3d max) : min_(std::move(min)), max_(std::move(max)) { }

            /**
             * @brief 检查两个长方体是否完全相等。
             *
             * @param rhs 要比较的另一个长方体。
             * @return 如果两个长方体的最小点和最大点都相等，则返回true；否则返回false。
             */
            bool operator==(const cuboid3d& rhs) const {
                return min_ == rhs.min_ and max_ == rhs.max_;
            }

            /**
             * @brief 获取长方体的最小顶点。
             *
             * @return 长方体的最小顶点。
             */
            [[nodiscard]] const point3d& min() const {
                return min_;
            }

            /**
             * @brief 获取长方体的最大顶点。
             *
             * @return 长方体的最大顶点。
             */
            [[nodiscard]] const point3d& max() const {
                return max_;
            }
            
        private:
            const point3d min_;
            const point3d max_;
        };

    } // rsmesh
} // geometry

#endif //RSMESH_CUBOID3D_H
