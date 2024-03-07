//
// Created by RainSure on 2023/10/25.
//

#ifndef RSMESH_POINT3D_H
#define RSMESH_POINT3D_H

#include "Eigen/Core"
#include "Eigen/Geometry"

namespace rsmesh {
    /**
     * \namespace rsmesh::geometry
     * \brief 该命名空间下主要定义了几何学相关的类和函数
     */
    namespace geometry {

        /**
         * @brief 2维向量
         */
        using vector2d = Eigen::RowVector2d;

        /**
         * @brief 2维点
         */
        using point2d = vector2d;
        /**
         * @brief 3维向量
         */
        using vector3d = Eigen::RowVector3d;
        /**
         * @brief 3维矩阵
         */
        using matrix3d = Eigen::Matrix<double, 3, 3, Eigen::RowMajor>;

        /**
         * @brief 3维点
         */
        using point3d = vector3d;

        /**
         * @brief 3维向量的集合
         */
        using vectors3d = Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor>;
        /**
         * @brief 3维点的集合
         */
        using points3d = vectors3d;

        /**
         * @brief 线性变换矩阵
         */
        using linear_transformation3d = Eigen::Matrix<double, 3, 3, Eigen::RowMajor>;

        /**
         * @brief 从一个变换矩阵中提取出线性变换矩阵
         * @tparam T 模板参数
         * @param t 变换矩阵
         * @return
         */
        template<class T>
        linear_transformation3d to_linear_transformation3d(T t) {
            return Eigen::Transform<double, 3, Eigen::Affine, Eigen::RowMajor>(t).linear();
        }

        /**
         * @brief 对一个三维点进行线性变换
         * @param lhs 线性变换矩阵
         * @param p 三维点
         * @return
         */
        inline
        point3d transform_point(const linear_transformation3d &lhs, const point3d &p) {
            return lhs * p.transpose();
        }

        /**
         * @brief 对一个三维向量进行线性变换
         * @param lhs  线性变换矩阵
         * @param v 三维向量
         * @return
         */
        inline
        vector3d transform_vector(const linear_transformation3d &lhs, const vector3d &v) {
            return lhs * v.transpose();
        }

    } // geometry
} // rsmesh

#endif //RSMESH_POINT3D_H
