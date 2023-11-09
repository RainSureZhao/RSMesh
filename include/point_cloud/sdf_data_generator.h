//
// Created by 赵润朔 on 2023/11/8.
//

#ifndef RSMESH_SDF_DATA_GENERATOR_H
#define RSMESH_SDF_DATA_GENERATOR_H

#include "geometry/point3d.h"
#include "types.h"

namespace rsmesh {
    namespace point_cloud {

        class sdf_data_generator {
        public:
            sdf_data_generator(const geometry::points3d& points, const geometry::vectors3d& normals, 
                               double min_distance, double max_distance, double multiplication = 2.0);
            
            [[nodiscard]] const geometry::points3d& sdf_points() const;
            [[nodiscard]] const valuesd& sdf_values() const;
            
        private:
            geometry::points3d sdf_points_;
            valuesd sdf_values_;
        };

    } // rsmesh
} // point_cloud

#endif //RSMESH_SDF_DATA_GENERATOR_H
