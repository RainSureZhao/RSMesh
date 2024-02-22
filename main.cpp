#include <string>
#include "examples/generate_sdf.h"
int main(int argc, char *argv[]) {
    const std::string points_with_normals = R"(E:\Code\Cpp_project\RSMesh\data\points_with_normals\bunny_normals.csv)";
    const std::string points_sdf = R"(E:\Code\Cpp_project\RSMesh\data\sdf\bunny_sdf.csv)";
    rsmesh::examples::generate_sdf(points_with_normals, points_sdf, 0.1);
    return 0;
}
