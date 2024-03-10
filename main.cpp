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
//        .output = R"(E:\Code\Cpp_project\RSMesh\data\surfaces\Bunny.obj)",
//        .rbf_name = "biharmonic3d",
//        .absolute_tolerance = 4e-2,
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
//    QApplication app(argc, argv);
//    Qt3DExtras::Qt3DWindow window;
//
//    // 创建根实体
//    auto *rootEntity = new Qt3DCore::QEntity;
//
//    // 加载模型
//    auto *mesh = new Qt3DRender::QMesh(rootEntity);
//    mesh->setSource(QUrl::fromLocalFile(R"(E:\Code\Cpp_project\RSMesh\data\surfaces\Bunny.obj)"));
//
//    // 创建一个材质
//    auto *material = new Qt3DExtras::QPhongMaterial(rootEntity);
//
//    // 为模型创建一个实体并应用材质和网格
//    auto *modelEntity = new Qt3DCore::QEntity(rootEntity);
//    modelEntity->addComponent(mesh);
//    modelEntity->addComponent(material);
//
//    // 创建一个相机
//    auto *camera = window.camera();
//    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
//    camera->setPosition(QVector3D(0, 0, 20.0f));
//    camera->setViewCenter(QVector3D(0, 0, 0));
//
//    // 创建相机控制器
//    auto *cameraController = new Qt3DExtras::QOrbitCameraController(rootEntity);
//    cameraController->setCamera(camera);
//
//    // 创建一个光源
//    auto *lightEntity = new Qt3DCore::QEntity(rootEntity);
//    auto *light = new Qt3DRender::QPointLight(lightEntity);
//    light->setColor("white");
//    light->setIntensity(1);
//    auto *lightTransform = new Qt3DCore::QTransform(lightEntity);
//    lightTransform->setTranslation(camera->position());
//    lightEntity->addComponent(light);
//    lightEntity->addComponent(lightTransform);
//
//    // 设置场景的根实体
//    window.setRootEntity(rootEntity);
//
//    // 显示窗口
//    window.show();
//
//    return app.exec();
    QGuiApplication app(argc, argv);

    // QSurfaceFormat::setDefaultFormat(QQuick3D::idealSurfaceFormat(4));
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/ui/Test.qml")); // 如果是从资源文件加载
    // 或者使用绝对路径加载QML文件，例如：
    // const QUrl url(QStringLiteral("file:///path/to/your/project/Main.qml"));
    engine.load(url);
    if (engine.rootObjects().isEmpty()) {
        std::cout << "false" << std::endl;
        return -1;
    }

    return app.exec();
}
