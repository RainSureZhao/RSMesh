//
// Created by 赵润朔 on 2024/3/11.
//

#ifndef RSMESH_FILELOADER_H
#define RSMESH_FILELOADER_H

#include <QObject>
#include "isosurface/surface.h"
#include "geometry/point3d.h"

namespace rsmesh::qt {
    class FileLoader : public QObject{
        Q_OBJECT
    public:
        explicit FileLoader(QObject *parent = nullptr);
    public slots:
        void openFile(const QString& filename);
    signals:
        void fileLoaded(const isosurface::surface& surface, const geometry::points3d& points);
    };

}


#endif //RSMESH_FILELOADER_H
