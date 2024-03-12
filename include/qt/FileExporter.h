//
// Created by 赵润朔 on 2024/3/11.
//

#ifndef RSMESH_FILEEXPORTER_H
#define RSMESH_FILEEXPORTER_H

#include <utility>
#include "QObject"
#include "isosurface/surface.h"
#include "geometry/point3d.h"

namespace rsmesh::qt {
    class FileExporter : public QObject{
        Q_OBJECT
    public:
        using DataCallback = std::function<void(std::function<void(rsmesh::isosurface::surface, rsmesh::geometry::points3d)>)>;
        explicit FileExporter(QObject *parent = nullptr);
        void setCallback(DataCallback callback_){
            this->callback = std::move(callback_);
        }
    public slots:
        void exportFile(const QString& filename, const QString& format);
    private:
        DataCallback callback;
    };
}



#endif //RSMESH_FILEEXPORTER_H
