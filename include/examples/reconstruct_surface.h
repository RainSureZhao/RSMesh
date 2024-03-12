//
// Created by RainSure on 2024/2/20.
//

#ifndef RSMESH_RECONSTRUCT_SURFACE_H
#define RSMESH_RECONSTRUCT_SURFACE_H

#include <optional>
#include "table.h"
#include "types.h"
#include "model.h"
#include "geometry/point3d.h"
#include "geometry/bbox3d.h"
#include "point_cloud/distance_filter.h"
#include "common/utils.h"
#include "interpolant.h"
#include "isosurface/isosurface.h"
#include "isosurface/rbf_field_function.h"
#include <iostream>

namespace rsmesh::examples {
    struct ReconstructionParameters {
        std::string input;
        std::string output;
        std::string rbf_name;
        double absolute_tolerance;
        geometry::bbox3d bbox;
        double mesh_resolution;
        std::vector<double> rbf_parameters;
        geometry::linear_transformation3d aniso = geometry::linear_transformation3d::Identity();
        double min_distance = 1e-4;
        double nugget = 0.0;
        int poly_degree = 0;
        int max_iterations = 30;
        bool inequality = false;
        bool reduce = false;
    };
    void reconstruct_surface(
        const ReconstructionParameters& params
            ) {
        tabled table = read_table(params.input);
        geometry::points3d points = table(Eigen::all, {0, 1, 2});
        valuesd values = table.col(3);
        std::optional<valuesd> values_lb;
        std::optional<valuesd> values_ub;
        if(params.inequality) {
            values_lb = table.col(4);
            values_ub = table.col(5);
        }

        // Remove very close points;

        std::cout << "The number of points before filter: " << points.rows() << std::endl;
        point_cloud::distance_filter filter(points, params.min_distance);
        std::tie(points, values) = filter(points, values);
        std::cout << "The number of points after filter: "  << points.rows() << std::endl;
        if(params.inequality) {
            *values_lb = filter(*values_lb);
            *values_ub = filter(*values_ub);
        }

        // Define the model
        auto rbf = common::make_rbf(params.rbf_name, params.rbf_parameters);
        rbf->set_anisotropy(params.aniso);
        model model(*rbf, 3, params.poly_degree);
        model.set_nugget(params.nugget);

        // Fit
        interpolant interpolant(model);
        if(params.inequality) {
            interpolant.fit_inequality(points, values, *values_lb, *values_ub, params.absolute_tolerance, params.max_iterations);
        } else if(params.reduce) {
            interpolant.fit_incrementally(points, values, params.absolute_tolerance, params.max_iterations);
        } else {
            interpolant.fit(points, values, params.absolute_tolerance, params.max_iterations);
        }

        // Generate the isosurfaces
        auto n_surface_points = (values.array() == 0.0).count();
        geometry::points3d surface_points(n_surface_points, 3);
        index_t si{};
        for(index_t i = 0; i < points.rows(); i ++) {
            if(values(i) == 0.0) {
                surface_points.row(si) = points.row(i);
                si++;
            }
        }
        isosurface::isosurface isosurface(params.bbox, params.mesh_resolution);
        isosurface::rbf_field_function field_function(interpolant);

        isosurface.generate_from_seed_points(surface_points, field_function).export_stl(params.output);
    }
}


#endif //RSMESH_RECONSTRUCT_SURFACE_H
