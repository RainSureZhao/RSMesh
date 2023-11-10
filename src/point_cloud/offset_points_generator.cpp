//
// Created by 赵润朔 on 2023/11/10.
//

#include "point_cloud/offset_points_generator.h"
#include "point_cloud/kdtree.h"
#include "types.h"

namespace rsmesh {
    namespace point_cloud {
        offset_points_generator::offset_points_generator(const geometry::points3d &points,
                                                         const geometry::vectors3d &normals, double offset) :
                offset_points_generator(points, normals, valuesd::Constant(points.rows(), offset)){}

        offset_points_generator::offset_points_generator(const geometry::points3d &points,
                                                         const geometry::vectors3d &normals, const valuesd &offsets) {
            if(normals.rows() != points.rows()) {
                throw std::invalid_argument("normals.rows() must be equal to points.rows().");
            }
            if(offsets.size() != points.size()) {
                throw std::invalid_argument("offsets.rows() must be equal to points.rows().");
            }
            
            kdtree tree(points, true);
            
            index_t n_points = points.rows();
            index_t n_new_points = 0;
            
            new_points_ = geometry::points3d(n_points, 3);
            new_normals_ = geometry::vectors3d(n_points, 3);
            
            for (index_t i = 0; i < n_points; i++) {
                auto p = points.row(i);
                auto n = normals.row(i);

                if (n == geometry::vector3d::Zero()) {
                    continue;
                }

                auto d = offsets(i);
                geometry::point3d q = p + d * n;

                auto [nn_indices, nn_distaces] = tree.knn_search(q, 1);
                auto i_nearest = nn_indices.at(0);

                if (i_nearest != i) {
                    continue;
                }

                new_points_.row(n_new_points) = q;
                new_normals_.row(n_new_points) = n;
                n_new_points ++;
            }

            new_points_.conservativeResize(n_new_points, 3);
            new_normals_.conservativeResize(n_new_points, 3);
        }

        const geometry::points3d &offset_points_generator::new_points() const {
            return new_points_;
        }

        const geometry::vectors3d &offset_points_generator::new_normals() const {
            return new_normals_;
        }
    } // rsmesh
} // point_cloud