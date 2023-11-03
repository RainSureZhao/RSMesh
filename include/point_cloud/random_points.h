//
// Created by 赵润朔 on 2023/11/3.
//

#ifndef RSMESH_RANDOM_POINTS_H
#define RSMESH_RANDOM_POINTS_H

#include <random>
#include "Eigen/Core"

#include "geometry/cuboid3d.h"
#include "geometry/point3d.h"
#include "geometry/sphere3d.h"
#include "types.h"

namespace rsmesh {
    namespace point_cloud {
        
        using seed_type = std::random_device::result_type;
        
        geometry::points3d random_points(const geometry::cuboid3d& cuboid, index_t n, seed_type seed = std::random_device()());
        
        geometry::points3d  random_points(const geometry::sphere3d& sphere, index_t n, seed_type seed = std::random_device()());

    } // rsmesh
} // point_cloud

#endif //RSMESH_RANDOM_POINTS_H
