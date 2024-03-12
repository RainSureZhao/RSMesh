//
// Created by 赵润朔 on 2024/3/11.
//

#include "qt/FileExporter.h"
#include "common/macros.h"
#include <QDebug>
#include "common/io.h"
rsmesh::qt::FileExporter::FileExporter(QObject *parent) : QObject(parent) {

}

void rsmesh::qt::FileExporter::exportFile(const QString &filename, const QString &format) {
    callback([filename, format](const rsmesh::isosurface::surface& surface, const rsmesh::geometry::points3d& points){
        std::string FilePath = filename.toStdString();
        // remove the file:/// prefix
        FilePath = FilePath.substr(8, FilePath.length() - 8);
        if(format == "obj"){
            surface.export_obj(FilePath);
        }
        else if(format == "stl") {
            surface.export_stl(FilePath);
        }
        else if(format == "xyz"){
            rsmesh::common::write_points(points, FilePath);
        }
        else{
            qDebug() << "Unsupported file format:" << format;
            RSMESH_UNREACHABLE();
        }
    });
}