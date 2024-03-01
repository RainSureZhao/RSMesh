//
// Created by 赵润朔 on 2024/2/23.
//

#ifndef RSMESH_RBF_FIELD_FUNCTION_H
#define RSMESH_RBF_FIELD_FUNCTION_H

#include "isosurface/field_function.h"
#include "interpolant.h"

namespace rsmesh::isosurface {
    class rbf_field_function : public field_function {
    public:
        explicit rbf_field_function(interpolant& interpolant) : interpolant_(interpolant) {}

        valuesd operator()(const geometry::points3d& points) const override {
            return interpolant_.evaluate_impl(points);
        }

        void set_evaluation_bbox(const geometry::bbox3d& bbox) override {
            interpolant_.set_evaluation_bbox_impl(bbox);
        }

    private:
        interpolant& interpolant_;
    };

}

#endif //RSMESH_RBF_FIELD_FUNCTION_H
