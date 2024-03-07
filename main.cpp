#include <string>
#include <iostream>
#include "examples/generate_sdf.h"
#include "examples/reconstruct_surface.h"
int main(int argc, char *argv[]) {
//    rsmesh::examples::generate_sdf(rsmesh::examples::SdfParameters{
//        .input = R"(E:\Code\Cpp_project\RSMesh\data\points_with_normals\bunny_normals.csv)",
//        .output = R"(E:\Code\Cpp_project\RSMesh\data\sdf\bunny_sdf1.csv)",
//        .max_offset = 0.01,
//        .min_offset = 0.0,
//        .sdf_multiplication = 2.0
//    });
    rsmesh::examples::reconstruct_surface(rsmesh::examples::ReconstructionParameters{
        .input = R"(E:\Code\Cpp_project\RSMesh\data\sdf\Bunny_sdf.csv)",
        .output = R"(E:\Code\Cpp_project\RSMesh\data\surfaces\Bunny.obj)",
        .rbf_name = "biharmonic3d",
        .absolute_tolerance = 4e-2,
        .bbox = rsmesh::geometry::bbox3d({-1.0, -1.0, -1.0}, {1.0, 1.0, 1.0}),
        .mesh_resolution = 0.001,
        .rbf_parameters = {1.0},
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
