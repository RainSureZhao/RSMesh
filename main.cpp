#include <iostream>
#include <vector>
#include "include/point_cloud/kdtree.h"
#include "rbf/rbf_base.h"
using namespace rsmesh::point_cloud;
using namespace rsmesh::geometry;

int main(int argc, char *argv[]) {
    points3d points;
    points.resize(10, 3);
    points << 0, 0, 0,
              1, 1, 1,
              2, 2, 2,
              3, 3, 3,
              4, 4, 4,
              5, 5, 5,
              6, 6, 6,
              7, 7, 7,
              8, 8, 8,
              9, 9, 9;

    kdtree tree(points, false);
    std::shared_ptr<rsmesh::rbf::rbf_base> ptr = nullptr;
    auto res = tree.knn_search(point3d(1.7, 1, 2), 3);
    for(const auto &p : res.second) {
        std::cout << p << std::endl;
    }
    std::cout << "Hello World" << std::endl;
    return 0;
}
