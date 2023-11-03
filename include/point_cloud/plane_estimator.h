//
// Created by 赵润朔 on 2023/11/3.
//

#ifndef RSMESH_PLANE_ESTIMATOR_H
#define RSMESH_PLANE_ESTIMATOR_H

#include "Eigen/Core"
#include "Eigen/SVD"
#include "geometry/point3d.h"

namespace rsmesh {
    namespace point_cloud {

        class plane_estimator {
        public:
            
            explicit plane_estimator(const geometry::points3d& points);
            
            double line_error() const;
            
            double plane_factor() const;
            
            geometry::vector3d  plane_normal() const;
            
            double plane_error() const;
            
            double point_error() const;
            
        private:
            static Eigen::JacobiSVD<Eigen::MatrixXd> pca_svd(const geometry::points3d& points);
        
            Eigen::Matrix3d  basis_;
            
            double point_err_;
            double line_err_;
            double plane_err_;
            double plane_factor_;
        };

    } // rsmesh
} // point_cloud

#endif //RSMESH_PLANE_ESTIMATOR_H
