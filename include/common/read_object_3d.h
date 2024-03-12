//
// Created by 赵润朔 on 2024/3/11.
//

#ifndef RSMESH_READ_OBJECT_3D_H
#define RSMESH_READ_OBJECT_3D_H

#include <fstream>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "geometry/point3d.h"
#include "isosurface/surface.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace rsmesh::common {
    using vertices_type = geometry::points3d;
    using face_type = Eigen::Matrix<index_t, 1, 3>;
    using faces_type = Eigen::Matrix<index_t, Eigen::Dynamic, 3, Eigen::RowMajor>;


    geometry::points3d read_points(const std::string& filename)  {
        std::ifstream ifs(filename);
        if(!ifs) {
            throw std::runtime_error("Failed to open file '" + filename + "'.");
        }
        std::string line;
        std::vector<geometry::point3d> points_;
        while(std::getline(ifs, line)) {
            if(line.empty() || line[0] == '#') {
                continue;
            }
            std::istringstream iss(line);
            geometry::point3d p;
            iss >> p.x() >> p.y() >> p.z();
            points_.push_back(p);
        }
        geometry::points3d points(points_.size(), 3);
        for(index_t i = 0; i < points_.size(); i ++) {
            points.row(i) = points_[i];
        }
        ifs.close();
        return points;
    }

    std::pair<geometry::points3d, geometry::vectors3d> read_points_with_normals(const std::string& filename) {
        std::ifstream ifs(filename);
        if(!ifs) {
            throw std::runtime_error("Failed to open file '" + filename + "'.");
        }
        std::string line;
        std::vector<geometry::point3d> points_;
        std::vector<geometry::vector3d> normals_;
        while(std::getline(ifs, line)) {
            if(line.empty() || line[0] == '#') {
                continue;
            }
            std::istringstream iss(line);
            geometry::point3d p;
            geometry::vector3d n;
            iss >> p.x() >> p.y() >> p.z() >> n.x() >> n.y() >> n.z();
            points_.push_back(p);
            normals_.push_back(n);
        }
        geometry::points3d points(points_.size(), 3);
        geometry::vectors3d normals(normals_.size(), 3);
        for(index_t i = 0; i < points_.size(); i ++) {
            points.row(i) = points_[i];
            normals.row(i) = normals_[i];
        }
        ifs.close();
        return {points, normals};
    }

    isosurface::surface read_surface(const std::string& filename, bool hasQuadrilateral = false) {
        std::ifstream ifs(filename);
        if(!ifs) {
            throw std::runtime_error("Failed to open file '" + filename + "'.");
        }
        Assimp::Importer importer;
        unsigned int flags = aiProcess_JoinIdenticalVertices; // Always join identical vertices
        if (hasQuadrilateral) {
            flags |= aiProcess_Triangulate; // Triangulate if the model has quadrilaterals
        }

        const aiScene* scene = importer.ReadFile(filename, flags);
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            throw std::runtime_error("Failed to load file '" + filename + "'.");
        }
        std::vector<geometry::point3d> points_;
        std::vector<face_type> faces_;
        for(index_t i = 0; i < scene->mNumMeshes; i ++) {
            const aiMesh* mesh = scene->mMeshes[i];
            for(index_t j = 0; j < mesh->mNumVertices; j ++) {
                geometry::point3d p;
                p.x() = mesh->mVertices[j].x;
                p.y() = mesh->mVertices[j].y;
                p.z() = mesh->mVertices[j].z;
                points_.push_back(p);
            }
            for(index_t j = 0; j < mesh->mNumFaces; j ++) {
                const aiFace& face = mesh->mFaces[j];
                if(face.mNumIndices != 3) {
                    throw std::runtime_error("Only triangle faces are supported.");
                }
                face_type f;
                f(0) = face.mIndices[0];
                f(1) = face.mIndices[1];
                f(2) = face.mIndices[2];
                faces_.push_back(f);
            }
        }
        vertices_type points(points_.size(), 3);
        faces_type faces(faces_.size(), 3);
        for(index_t i = 0; i < points_.size(); i ++) {
            points.row(i) = points_[i];
        }
        for(index_t i = 0; i < faces_.size(); i ++) {
            faces.row(i) = faces_[i];
        }
        return {points, faces};
    }
}

#endif //RSMESH_READ_OBJECT_3D_H
