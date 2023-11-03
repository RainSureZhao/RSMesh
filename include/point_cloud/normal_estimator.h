//
// Created by 赵润朔 on 2023/11/3.
//

#ifndef RSMESH_NORMAL_ESTIMATOR_H
#define RSMESH_NORMAL_ESTIMATOR_H

#include <vector>
#include "Eigen/Core"

#include "geometry/point3d.h"
#include "point_cloud/kdtree.h"
#include "types.h"

namespace rsmesh {
    namespace point_cloud {

        class normal_estimator {
        public:
            explicit normal_estimator(const geometry::points3d& points);
            
            normal_estimator& estimate_with_knn(index_t k, double plane_factor_threshold = 1.8);
            
            normal_estimator& estimate_with_radius(double radius, double plane_factor_threshold = 1.8);
            
            geometry::vectors3d  orient_by_outward_vector(const geometry::vector3d& v);
            
        private:
            [[nodiscard]] geometry::vector3d estimate_impl(const std::vector<index_t>& nn_indices, double plane_factor_threshold) const;
            
            const index_t n_points_;
            const geometry::points3d points_;
            kdtree tree_;
            
            geometry::vectors3d normals_;
        };

    } // rsmesh
} // point_cloud

#endif //RSMESH_NORMAL_ESTIMATOR_H
