//
// Created by RainSure on 2024/2/22.
//

#ifndef RSMESH_UTILS_H
#define RSMESH_UTILS_H

#include "rbf/biharmonic2d.h"
#include "rbf/biharmonic3d.h"

namespace rsmesh::common {

    /**
     * @brief 生成一个RBF（径向基函数）对象
     *
     * 此函数根据指定的RBF类型和参数，创建并返回一个相应的RBF对象。目前支持的RBF类型包括
     * "biharmonic2d" 和 "biharmonic3d"。每种类型的RBF对象都是`rsmesh::rbf::rbf_base`的派生类，
     * 用于不同的数学和工程计算中，特别是在插值和形状参数化等领域。
     *
     * 若要了解更多关于径向基函数的信息，请参考
     * [维基百科](https://en.wikipedia.org/wiki/Radial_basis_function)。
     *
     * @param rbf_type 指定RBF类型的字符串。有效值包括："biharmonic2d" 和 "biharmonic3d"。
     * @param params RBF对象构造需要的参数列表，其内容和数量取决于RBF类型。
     *
     * @return 返回一个指向新创建的RBF对象的`std::unique_ptr<rsmesh::rbf::rbf_base>`智能指针。
     *
     * @throws std::invalid_argument 如果提供了未知的RBF类型，则抛出此异常。
     */
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
