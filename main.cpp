#include <string>
#include "examples/closed_normals.h"
#include "examples/generate_sdf.h"
int main(int argc, char *argv[]) {
    const std::string points_input = R"(E:\Code\Cpp_project\RSMesh\data\point_cloud\bunny.csv)";
    const std::string points_with_normals = R"(E:\Code\Cpp_project\RSMesh\data\points_with_normals\bunny_normals.csv)";
    rsmesh::examples::generate_closed_normals(points_input, points_with_normals);
    return 0;
}
