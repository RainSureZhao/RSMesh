//
// Created by 赵润朔 on 2024/2/19.
//

#ifndef RSMESH_CLOSED_NORMALS_H
#define RSMESH_CLOSED_NORMALS_H

#include "types.h"
#include "table.h"
#include "geometry/point3d.h"
#include "point_cloud/normal_estimator.h"
#include "common/eigen_utility.h"
#include <string>
#include <vector>
#include <exception>
#include <iostream>

namespace rsmesh::examples {
    void generate_closed_normals(
            const std::string& input,
            const std::string& output,
            const std::vector<index_t>& knn_number_normals = {10, 30, 100, 300},
            const index_t knn_number_orientations = 20,
            const double min_plane_factor = 1.8
            ) {
        try {
            // read points x, y, z
            tabled table = read_table(input);
            geometry::points3d points = table(Eigen::all, {0, 1, 2});

            // get normals
            geometry::vectors3d normals = point_cloud::normal_estimator(points).
                    estimate_with_knn(knn_number_normals, min_plane_factor).
                    orient_closed_surface(knn_number_orientations);

            // output points with normals
            write_table(output, common::concatenate_cols(points, normals));
        } catch(const std::exception& e) {
            std::cerr << e.what() << std::endl;
            exit(1);
        }
    }
} // namespace rsmesh::examples

#endif //RSMESH_CLOSED_NORMALS_H
