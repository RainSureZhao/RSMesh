//
// Created by 赵润朔 on 2023/10/25.
//

#ifndef RSMESH_SPHERE3D_H
#define RSMESH_SPHERE3D_H

#include <utility>

#include "point3d.h"

namespace rsmesh {
    namespace geometry {

        class sphere3d {
        public:
            sphere3d() : center_(point3d::Zero()) {
                
            }
            sphere3d(point3d center, const double radius) : center_(std::move(center)), radius_(radius) {
                
            }
            
            bool operator==(const sphere3d& rhs) const {
                return center_ == rhs.center_ and radius_ == rhs.radius_;
            }
            
            [[nodiscard]] const point3d& center() const {
                return center_;
            }
            
            [[nodiscard]] const double radius() const {
                return radius_;
            }

            
        private:
            const point3d center_;
            const double radius_{1.0};
        };

    } // rsmesh
} // geometry

#endif //RSMESH_SPHERE3D_H
