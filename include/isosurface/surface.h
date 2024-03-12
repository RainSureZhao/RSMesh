//
// Created by RainSure on 2024/2/23.
//

#ifndef RSMESH_SURFACE_H
#define RSMESH_SURFACE_H

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "geometry/point3d.h"
#include "numeric/roundtrip_string.h"
#include "types.h"

namespace rsmesh::isosurface {
    class surface {
    public:
        using vertices_type = geometry::points3d;
        using face_type = Eigen::Matrix<index_t, 1, 3>;
        using faces_type = Eigen::Matrix<index_t, Eigen::Dynamic, 3, Eigen::RowMajor>;

        surface() = default;

        surface(const vertices_type& vertices, const faces_type& faces)
                : vertices_(vertices.rows(), 3), faces_(faces.rows(), 3) {
            std::vector<index_t> vi_map(vertices.rows(), -1);
            face_type new_face;

            auto v_it = vertices_.rowwise().begin();
            auto f_it = faces_.rowwise().begin();
            index_t n_vertices = 0;
            index_t n_faces = 0;

            for (auto face : faces.rowwise()) {
                for (auto i = 0; i < 3; i++) {
                    auto& vi = vi_map.at(face(i));
                    if (vi == -1) {
                        vi = n_vertices;
                        *v_it++ = vertices.row(face(i));
                        n_vertices++;
                    }
                    new_face(i) = vi;
                }
                *f_it++ = new_face;
                n_faces++;
            }

            vertices_.conservativeResize(n_vertices, 3);
            faces_.conservativeResize(n_faces, 3);
        }

        void export_obj(const std::string& filename) const {
            std::ofstream ofs(filename);
            if (!ofs) {
                throw std::runtime_error("Failed to open file '" + filename + "'.");
            }

            for (auto v : vertices_.rowwise()) {
                ofs << "v " << numeric::to_string(v(0)) << ' ' << numeric::to_string(v(1)) << ' '
                    << numeric::to_string(v(2)) << '\n';
            }

            for (auto f : faces_.rowwise()) {
                ofs << "f " << f(0) + 1 << ' ' << f(1) + 1 << ' ' << f(2) + 1 << '\n';
            }
            ofs.close();
        }

        void export_stl(const std::string& filename) const {
            std::fstream ofs(filename);
            if(!ofs) {
                throw std::runtime_error("Failed to open file '" + filename + "'.");
            }
            ofs << "solid mesh" << std::endl;
            for (auto f : faces_.rowwise()) {
                auto v0 = vertices_.row(f(0));
                auto v1 = vertices_.row(f(1));
                auto v2 = vertices_.row(f(2));
                auto n = (v1 - v0).cross(v2 - v0).normalized();
                ofs << "facet normal " << numeric::to_string(n(0)) << ' ' << numeric::to_string(n(1)) << ' '
                    << numeric::to_string(n(2)) << std::endl;
                ofs << "outer loop" << std::endl;
                ofs << "vertex " << numeric::to_string(v0(0)) << ' ' << numeric::to_string(v0(1)) << ' '
                    << numeric::to_string(v0(2)) << std::endl;
                ofs << "vertex " << numeric::to_string(v1(0)) << ' ' << numeric::to_string(v1(1)) << ' '
                    << numeric::to_string(v1(2)) << std::endl;
                ofs << "vertex " << numeric::to_string(v2(0)) << ' ' << numeric::to_string(v2(1)) << ' '
                    << numeric::to_string(v2(2)) << std::endl;
                ofs << "endloop" << std::endl;
                ofs << "endfacet" << std::endl;
            }
            ofs << "endsolid mesh" << std::endl;
            ofs.close();
        }


        [[nodiscard]] const faces_type& faces() const { return faces_; }

        [[nodiscard]] const vertices_type& vertices() const { return vertices_; }

    private:
        vertices_type vertices_;
        faces_type faces_;
    };
} // namespace rsmesh::isosurface


#endif //RSMESH_SURFACE_H
