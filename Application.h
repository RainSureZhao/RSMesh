//
// Created by 赵润朔 on 2024/3/11.
//

#ifndef RSMESH_APPLICATION_H
#define RSMESH_APPLICATION_H
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>
#include "qt/FileLoader.h"
#include "qt/FileExporter.h"
#include "qt/ModelConverter.h"
#include "isosurface/surface.h"
#include "geometry/point3d.h"
#include "qt/ModelConverter.h"

class Application : public QObject {
    Q_OBJECT
public:
    Application(int argc, char *argv[])
            : app(argc, argv), engine(), fileLoader(), fileExporter() {
        QString appDirPath = R"(D:\Code\cpp\RSMesh)";
        engine.rootContext()->setContextProperty("appDirPath", appDirPath);
        engine.rootContext()->setContextProperty("fileLoader", &fileLoader);
        engine.rootContext()->setContextProperty("fileExporter", &fileExporter);
        engine.rootContext()->setContextProperty("modelConverter", &modelConverter);
        qmlRegisterType<rsmesh::qt::ModelConverter>("ModelConversion", 1, 0, "ModelConverter");
        connect(&fileLoader, &rsmesh::qt::FileLoader::fileLoaded, this, &Application::onFileLoaded);
        // 此处加载你的QML文件等其他初始化代码
        engine.load(QUrl(QStringLiteral("qrc:/ui/MainWindow.qml")));
    }
    int run() {
        if (engine.rootObjects().isEmpty())
            return -1;
        return app.exec();
    }

public slots:
    void onFileLoaded(const rsmesh::isosurface::surface &surface_, const rsmesh::geometry::points3d &points_) {
        hasModel = true;
        this->surface = surface_;
        this->points = points_;
        fileExporter.setCallback([this](const std::function<void(const rsmesh::isosurface::surface&, const rsmesh::geometry::points3d&)>& callback) {
            callback(this->surface, this->points);
        });
    };

public:
    Q_INVOKABLE [[nodiscard]] rsmesh::isosurface::surface getSurface() const { return surface; }
    Q_INVOKABLE [[nodiscard]] rsmesh::geometry::points3d getPoints() const { return points; }
private:
    QGuiApplication app;
    QQmlApplicationEngine engine;
    rsmesh::qt::FileLoader fileLoader;
    rsmesh::qt::FileExporter fileExporter;
    rsmesh::qt::ModelConverter modelConverter;
    bool hasModel = false; // 判断是否打开了模型，只有打开后，导出按钮才能使用，否则是灰色的
    rsmesh::isosurface::surface surface; // 当前打开的模型的曲面，包括面、点信息
    rsmesh::geometry::points3d points; // 当前打开的模型的点云信息
};

#endif //RSMESH_APPLICATION_H
