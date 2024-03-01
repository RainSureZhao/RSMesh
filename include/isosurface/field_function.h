//
// Created by 赵润朔 on 2024/2/23.
//

#ifndef RSMESH_FIELD_FUNCTION_H
#define RSMESH_FIELD_FUNCTION_H

#include "geometry/bbox3d.h"
#include "geometry/point3d.h"
#include "types.h"

namespace rsmesh::isosurface {
    class field_function {
    public:
        virtual ~field_function() = default;

        field_function(const field_function&) = delete;
        field_function(field_function&&) = delete;
        field_function& operator=(const field_function&) = delete;
        field_function& operator=(field_function&&) = delete;

        virtual valuesd operator()(const geometry::points3d& points) const = 0;

        virtual void set_evaluation_bbox(const geometry::bbox3d& /*bbox*/) {}

    protected:
        field_function() = default;
    };
}


#endif //RSMESH_FIELD_FUNCTION_H
