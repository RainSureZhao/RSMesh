//
// Created by 赵润朔 on 2023/10/25.
//

#ifndef RSMESH_CUBOID3D_H
#define RSMESH_CUBOID3D_H

#include "point3d.h"

namespace rsmesh {
    namespace geometry {

        class cuboid3d {
        public:
            cuboid3d() : min_(point3d::Zero()), max_(point3d::Ones()) { }
            cuboid3d(point3d min, point3d max) : min_(std::move(min)), max_(std::move(max)) { }
            
            bool operator==(const cuboid3d& rhs) const {
                return min_ == rhs.min_ and max_ == rhs.max_;
            }
            
            [[nodiscard]] const point3d& min() const {
                return min_;
            }
            
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
