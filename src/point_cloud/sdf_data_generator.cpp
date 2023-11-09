//
// Created by 赵润朔 on 2023/11/8.
//

#include "point_cloud/sdf_data_generator.h"
#include "point_cloud/kdtree.h"

namespace rsmesh {
    namespace point_cloud {
        sdf_data_generator::sdf_data_generator(const geometry::points3d &points, const geometry::vectors3d &normals,
                                               double min_distance, double max_distance, double multiplication) {
            if(normals.rows() != points.rows()) {
                throw std::invalid_argument("normals.rows() must be equal to points.rows()");
            }
            
            if(min_distance > max_distance) {
                throw std::invalid_argument("min_distance must be less than or equal to max_distance");
            }
            
            if(multiplication <= 1.0 || multiplication > 3.0) {
                throw std::invalid_argument("multiplication must be within the range (1.0, 3.0]");
            }
            
            kdtree tree(points, true);
            
            auto n_points = points.rows();
            auto n_reduced_points = 
                    static_cast<index_t>(((multiplication - 1.0) / 2.0 * static_cast<double>(n_points)));
        }

        const geometry::points3d &sdf_data_generator::sdf_points() const {
            return sdf_points_;
        }

        const valuesd &sdf_data_generator::sdf_values() const {
            return sdf_values_;
        }
    } // rsmesh
} // point_cloud