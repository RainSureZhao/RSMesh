//
// Created by 赵润朔 on 2024/2/23.
//

#ifndef RSMESH_RBF_FIELD_FUNCTION_25D_H
#define RSMESH_RBF_FIELD_FUNCTION_25D_H

#include "field_function.h"
#include "geometry/point3d.h"
#include "interpolant.h"
#include "types.h"

namespace rsmesh::isosurface {
    class rbf_field_function_25d : public field_function {
    public:
        explicit rbf_field_function_25d(interpolant& interpolant) : interpolant_(interpolant) {}

        valuesd operator()(const geometry::points3d& points) const override {
            geometry::points3d points_2d(points);
            points_2d.col(2).array() = 0.0;

            return points.col(2) - interpolant_.evaluate_impl(points_2d);
        }

        void set_evaluation_bbox(const geometry::bbox3d& bbox) override {
            interpolant_.set_evaluation_bbox_impl(bbox);
        }

    private:
        interpolant& interpolant_;
    };
}

#endif //RSMESH_RBF_FIELD_FUNCTION_25D_H
