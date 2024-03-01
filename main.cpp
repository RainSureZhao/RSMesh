#include <string>
#include <iostream>
#include "examples/reconstruct_surface.h"
int main(int argc, char *argv[]) {
    rsmesh::example::reconstruct_surface(rsmesh::example::ReconstructionParameters{
        .input = R"(E:\Code\Cpp_project\RSMesh\data\sdf\bunny_sdf.csv)",
        .output = R"(E:\Code\Cpp_project\RSMesh\data\surfaces\bunny.obj)",
        .rbf_name = "biharmonic2d",
        .absolute_tolerance = 1e-6,
        .bbox = rsmesh::geometry::bbox3d({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}),
        .mesh_resolution = 0.01,
        .rbf_parameters = {},
        .aniso = rsmesh::geometry::linear_transformation3d::Identity(),
        .min_distance = 1e-10,
        .nugget = 0.0,
        .poly_degree = 0,
        .max_iterations = 30,
        .inequality = false,
        .reduce = false
    });

    return 0;
}
