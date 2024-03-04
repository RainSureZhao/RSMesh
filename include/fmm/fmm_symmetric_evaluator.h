//
// Created by RainSure on 2024/2/4.
//

#ifndef RSMESH_FMM_SYMMETRIC_EVALUATOR_H
#define RSMESH_FMM_SYMMETRIC_EVALUATOR_H

#include "model.h"
#include "geometry/bbox3d.h"

namespace rsmesh::fmm {
    template <int Order>
    class fmm_symmetric_evaluator {
    public:
        fmm_symmetric_evaluator(const model& model, int tree_height, const geometry::bbox3d& bbox);

        ~fmm_symmetric_evaluator();

        fmm_symmetric_evaluator(const fmm_symmetric_evaluator&) = delete;
        fmm_symmetric_evaluator(fmm_symmetric_evaluator&&) = delete;
        fmm_symmetric_evaluator& operator=(const fmm_symmetric_evaluator&) = delete;
        fmm_symmetric_evaluator& operator=(fmm_symmetric_evaluator&&) = delete;

        [[nodiscard]] valuesd evaluate() const;

        void set_points(const geometry::points3d& points);

        void set_weights(const Eigen::Ref<const valuesd>& weights);

    private:
        class impl;

        std::unique_ptr<impl> pimpl_;
    };
}

#endif //RSMESH_FMM_SYMMETRIC_EVALUATOR_H
