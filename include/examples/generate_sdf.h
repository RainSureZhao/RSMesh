//
// Created by 赵润朔 on 2024/2/19.
//

#ifndef RSMESH_GENERATE_SDF_H
#define RSMESH_GENERATE_SDF_H

#include <string>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include "types.h"
#include "table.h"
#include "geometry/point3d.h"
#include "common/utility.h"
#include "point_cloud/sdf_data_generator.h"
#include "common/eigen_utility.h"

namespace rsmesh::examples {
    struct SdfParameters {
        std::string input;
        std::string output;
        double max_offset;
        double min_offset = 0.0;
        double sdf_multiplication = 2.0;
    };
    void generate_sdf(
        const SdfParameters& parameters
            ) {
        const std::string& input = parameters.input;
        const std::string& output = parameters.output;
        const double max_offset = parameters.max_offset;
        const double min_offset = parameters.min_offset;
        const double sdf_multiplication = parameters.sdf_multiplication;
        // read data: points(x, y, z) and normals(nx, ny, nz)
        tabled table = read_table(input);

        // Shuffle the points so that the off-surface points will not be clustered
        std::vector<index_t> indices(table.rows());
        std::iota(indices.begin(), indices.end(), index_t{0});
        std::shuffle(indices.begin(), indices.end(), std::mt19937{std::random_device{}()});
        table = table(indices, Eigen::all).eval();

        geometry::points3d surface_points = table(Eigen::all, {0, 1, 2});
        geometry::vectors3d surface_normals = table(Eigen::all, {3, 4, 5});

        // generate SDF (signed distance function) data
        point_cloud::sdf_data_generator sdf_data(surface_points, surface_normals, min_offset, max_offset, sdf_multiplication);
        geometry::points3d points = sdf_data.sdf_points();
        valuesd values = sdf_data.sdf_values();

        // Save the SDF data, x y z val
        write_table(output, common::concatenate_cols(points, values));
    }
}

#endif //RSMESH_GENERATE_SDF_H
