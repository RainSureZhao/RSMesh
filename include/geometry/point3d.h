//
// Created by 赵润朔 on 2023/10/25.
//

#ifndef RSMESH_POINT3D_H
#define RSMESH_POINT3D_H

#include "Eigen/Core"
#include "Eigen/Geometry"

namespace rsmesh {
    namespace geometry {
        
        using vector3d = Eigen::RowVector3d;
        
        using point3d = vector3d;
            
        using vectors3d = Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor>;
        
        using points3d = vectors3d;
        
        using linear_transformation3d = Eigen::Matrix<double, 3, 3, Eigen::RowMajor>;
        
        template<class T>
        linear_transformation3d to_linear_transformation3d(T t) {
            return Eigen::Transform<double, 3, Eigen::Affine, Eigen::RowMajor>(t).linear();
        }
        
        inline
        point3d transform_point(const linear_transformation3d &lhs, const point3d &p) {
            return lhs * p.transpose();
        }
        
        inline
        vector3d transform_vector(const linear_transformation3d &lhs, const vector3d &v) {
            return lhs * v.transpose();
        }

    } // rsmesh
} // geometry

#endif //RSMESH_POINT3D_H
