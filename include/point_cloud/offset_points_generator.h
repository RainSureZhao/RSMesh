//
// Created by 赵润朔 on 2023/11/10.
//

#ifndef RSMESH_OFFSET_POINTS_GENERATOR_H
#define RSMESH_OFFSET_POINTS_GENERATOR_H

#include "geometry/point3d.h"
#include "types.h"

namespace rsmesh {
    namespace point_cloud {

        class offset_points_generator {
        public:
            offset_points_generator(const geometry::points3d& points, const geometry::vectors3d& normals,
                                    double offset);
            
            offset_points_generator(const geometry::points3d& points, const geometry::vectors3d& normals,
                                    const valuesd& offsets);
            
            [[nodiscard]] const geometry::points3d& new_points() const;
            
            [[nodiscard]] const geometry::vectors3d& new_normals() const;
            
        private:
            geometry::points3d new_points_;
            geometry::vectors3d new_normals_;
        };

    } // rsmesh
} // point_cloud

#endif //RSMESH_OFFSET_POINTS_GENERATOR_H
