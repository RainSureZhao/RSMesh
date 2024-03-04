//
// Created by RainSure on 2024/2/19.
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

/**
 * \namespace rsmesh::examples
 * \brief examples命名空间，包含一些使用RSMesh库的示例
 */
namespace rsmesh::examples {
    struct ClosedNormalsParameters {
        std::string input;
        std::string output;
        std::vector<index_t> knn_number_normals = {10, 30, 100, 300};
        index_t knn_number_orientations = 20;
        double min_plane_factor = 1.8;
    };
    void generate_closed_normals(
            const ClosedNormalsParameters& parameters
            ) {
        try {
            const std::string& input = parameters.input;
            const std::string& output = parameters.output;
            const std::vector<index_t>& knn_number_normals = parameters.knn_number_normals;
            const index_t knn_number_orientations = parameters.knn_number_orientations;
            const double min_plane_factor = parameters.min_plane_factor;
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
