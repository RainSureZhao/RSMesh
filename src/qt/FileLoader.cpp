//
// Created by 赵润朔 on 2024/3/11.
//

#include "qt/FileLoader.h"
#include "common/read_object_3d.h"
#include <QDebug>
#include "common/macros.h"
rsmesh::qt::FileLoader::FileLoader(QObject *parent) : QObject(parent){

}

void rsmesh::qt::FileLoader::openFile(const QString &filename) {
    std::string FilePath = filename.toStdString();
    // remove the file:/// prefix
    FilePath = FilePath.substr(8, FilePath.length() - 8);
    std::string format = FilePath.find_last_of('.') == std::string::npos ? "" : FilePath.substr(FilePath.find_last_of('.') + 1);
    if(format == "obj" or format == "stl"){
        auto surface = rsmesh::common::read_surface(FilePath);
        emit fileLoaded(surface, surface.vertices());
    }
    else if(format == "xyz" or format == "csv"){
        auto points = rsmesh::common::read_points(FilePath);
        emit fileLoaded(isosurface::surface(), points);
    }
    else{
        qDebug() << "Unsupported file format:" << format.c_str();
        RSMESH_UNREACHABLE();
    }
}
