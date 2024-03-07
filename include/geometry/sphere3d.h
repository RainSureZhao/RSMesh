//
// Created by RainSure on 2023/10/25.
//

#ifndef RSMESH_SPHERE3D_H
#define RSMESH_SPHERE3D_H

#include <utility>
#include "point3d.h"

namespace rsmesh {
    namespace geometry {

        /**
         * @brief 三维空间中的球体表示。
         *
         * `sphere3d`类提供了一个三维空间中的球体的简单抽象，通过定义球体的中心点和半径来表示。
         * 这种表示方法使得计算球体的体积、表面积以及执行与球体相关的几何操作变得简单直接。
         *
         * @note 球体的半径被假设为非负值。
         */
        class sphere3d {
        public:
            /**
             * @brief 默认构造函数，创建一个单位球体。
             *
             * 初始化一个原点处的中心和默认半径为1的球体。
             */
            sphere3d() : center_(point3d::Zero()) {
                
            }
            /**
             * @brief 参数化构造函数，通过指定中心点和半径来创建球体。
             *
             * @param center 球体的中心点。
             * @param radius 球体的半径，应为非负值。
             */
            sphere3d(point3d center, const double radius) : center_(std::move(center)), radius_(radius) {
                
            }

            /**
             * @brief 检查两个球体是否完全相等。
             *
             * @param rhs 要比较的另一个球体。
             * @return 如果两个球体的中心点和半径都相等，则返回true；否则返回false。
             */
            bool operator==(const sphere3d& rhs) const {
                return center_ == rhs.center_ and radius_ == rhs.radius_;
            }

            /**
             * @brief 获取球体的中心点。
             *
             * @return 球体的中心点。
             */
            [[nodiscard]] const point3d& center() const {
                return center_;
            }

            /**
             * @brief 获取球体的半径。
             *
             * @return 球体的半径。
             */
            [[nodiscard]] const double radius() const {
                return radius_;
            }

            
        private:
            const point3d center_; ///< 球体的中心点。
            const double radius_{1.0}; ///< 球体的半径，默认为1.0。
        };

    } // rsmesh
} // geometry

#endif //RSMESH_SPHERE3D_H
