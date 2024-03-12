#include <string>
#include <iostream>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QPointLight>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QPhongMaterial>
#include <QApplication>
#include "examples/generate_sdf.h"
#include "examples/reconstruct_surface.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtGui>
#include <QtQuick3D/qquick3d.h>

#include <CGAL/Simple_cartesian.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
// 包括其他必要的CGAL和Qt头文件
#include "Application.h"

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;

int main(int argc, char *argv[]) {
//    rsmesh::examples::generate_sdf(rsmesh::examples::SdfParameters{
//        .input = R"(E:\Code\Cpp_project\RSMesh\data\points_with_normals\bunny_normals.csv)",
//        .output = R"(E:\Code\Cpp_project\RSMesh\data\sdf\bunny_sdf1.csv)",
//        .max_offset = 0.01,
//        .min_offset = 0.0,
//        .sdf_multiplication = 2.0
//    });
//    rsmesh::examples::reconstruct_surface(rsmesh::examples::ReconstructionParameters{
//        .input = R"(E:\Code\Cpp_project\RSMesh\data\sdf\Bunny_sdf.csv)",
//        .output = R"(E:\Code\Cpp_project\RSMesh\data\surfaces\Bunny.stl)",
//        .rbf_name = "biharmonic3d",
//        .absolute_tolerance = 0.1,
//        .bbox = rsmesh::geometry::bbox3d({-1.0, -1.0, -1.0}, {1.0, 1.0, 1.0}),
//        .mesh_resolution = 0.001,
//        .rbf_parameters = {1.0},
//        .aniso = rsmesh::geometry::linear_transformation3d::Identity(),
//        .min_distance = 1e-10,
//        .nugget = 0.0,
//        .poly_degree = 0,
//        .max_iterations = 30,
//        .inequality = false,
//        .reduce = false
//    });
//    return 0;
    Application app(argc, argv);
    return app.run();
}
