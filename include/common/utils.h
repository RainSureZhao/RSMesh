//
// Created by RainSure on 2024/2/22.
//

#ifndef RSMESH_UTILS_H
#define RSMESH_UTILS_H

#include "rbf/biharmonic2d.h"
#include "rbf/biharmonic3d.h"

namespace rsmesh::common {

    inline std::unique_ptr<rsmesh::rbf::rbf_base> make_rbf(const std::string& rbf_type,
                                                           const std::vector<double>& params) {
        if(rbf_type == "biharmonic2d") {
            return std::make_unique<rsmesh::rbf::biharmonic2d>(params);
        }
        if(rbf_type == "biharmonic3d") {
            return std::make_unique<rsmesh::rbf::biharmonic3d>(params);
        }

        throw std::invalid_argument("Unknown RBF type: " + rbf_type);
    }
} // rsmesh::common


#endif //RSMESH_UTILS_H
