//
// Created by RainSure on 2024/2/4.
//

#ifndef RSMESH_FMM_EVALUATOR_H
#define RSMESH_FMM_EVALUATOR_H

#include <memory>
#include "Eigen/Core"
#include "geometry/bbox3d.h"
#include "geometry/point3d.h"
#include "model.h"
#include "types.h"
namespace rsmesh::fmm {
    template<int Order>
    class fmm_evaluator {
    public:
        fmm_evaluator(const model& model, int tree_height, const geometry::bbox3d& bbox);
        ~fmm_evaluator();

        fmm_evaluator(const fmm_evaluator&) = delete;
        fmm_evaluator(fmm_evaluator&&) = delete;
        fmm_evaluator& operator=(const fmm_evaluator&) = delete;
        fmm_evaluator& operator=(fmm_evaluator&&) = delete;

        [[nodiscard]] valuesd evaluate() const;

        void set_field_points(const geometry::points3d& points);

        void set_source_points(const geometry::points3d& points);

        void set_source_points_and_weights(const geometry::points3d& points,
                                           const Eigen::Ref<const valuesd>& weights);

        void set_weights(const Eigen::Ref<const valuesd>& weights);
    private:
        class impl;
        std::unique_ptr<impl> pimpl_;
    };
} // namespace rsmesh::fmm




#endif //RSMESH_FMM_EVALUATOR_H
