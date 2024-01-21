//
// Created by 赵润朔 on 2023/10/26.
//

#ifndef RSMESH_BBOX3D_H
#define RSMESH_BBOX3D_H

#include "point3d.h"
#include "common/macros.h"

namespace rsmesh {
    namespace geometry {

        class bbox3d {
        public:
            bbox3d();
            
            bbox3d(const point3d& min, const point3d &max);
            
            bool operator== (const bbox3d& rhs) const;
            
            point3d center() const;
            
            bool contains(const point3d& p) const;
            
            const point3d& max() const;
            
            const point3d& min() const;
            
            vector3d size() const;
            
            bbox3d transform(const linear_transformation3d& t) const;
            
            bbox3d union_hull(const bbox3d& other) const;
            
            static bbox3d from_points(const points3d& points);
            
            
            template<class InputIterator>
            static bbox3d from_points(InputIterator points_begin, InputIterator points_end) {
                bbox3d result;
                if(points_begin == points_end) return result;
                auto it = points_begin;
                auto first_point = *it;
                result.min_(0) = result.max_(0) = first_point(0);
                result.min_(1) = result.max_(1) = first_point(1);
                result.min_(2) = result.max_(2) = first_point(2);
                
                for( ; it != points_end; it ++) {
                    auto point = *it;
                    result.min_(0) = std::min(result.min_(0), point(0));
                    result.min_(1) = std::min(result.min_(1), point(1));
                    result.min_(2) = std::min(result.min_(2), point(2));
                    result.max_(0) = std::max(result.max_(0), point(0));
                    result.max_(1) = std::max(result.max_(1), point(1));
                    result.max_(2) = std::max(result.max_(2), point(2));
                }
                
                return result;
            }
            
        private:
            point3d min_;
            point3d max_;
        };

    } // rsmesh
} // geometry

#endif //RSMESH_BBOX3D_H
