//
// Created by RainSure on 2024/2/29.
//
#include "interpolation/rbf_incremental_fitter.h"
#include "point_cloud/distance_filter.h"
#include "boost/range/irange.hpp"
#include "common/zip_sort.h"
#include "fmm/fmm_tree_height.h"
#include "interpolation/rbf_incremental_fitter.h"
#include "interpolation/rbf_solver.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <unordered_set>


namespace rsmesh::interpolation {
    rbf_incremental_fitter::rbf_incremental_fitter(const model& model, const geometry::points3d& points)
            : model_(model),
              points_(points),
              n_points_(points.rows()),
              n_poly_basis_(model.poly_basis_size()),
              bbox_(geometry::bbox3d::from_points(points)) {}

    std::pair<std::vector<index_t>, valuesd> rbf_incremental_fitter::fit(
            const valuesd& values, double absolute_tolerance, int max_iter) const {
        auto filtering_distance = bbox_.size().mean() / 4.0;

        auto centers = point_cloud::distance_filter(points_, filtering_distance).filtered_indices();
        auto n_centers = static_cast<index_t>(centers.size());
        valuesd center_weights = valuesd::Zero(n_centers + n_poly_basis_);

        std::unique_ptr<rbf_solver> solver;
        std::unique_ptr<rbf_evaluator<>> res_eval;
        auto last_tree_height = 0;

        while (true) {
            std::cout << "Number of RBF centers: " << n_centers << " / " << n_points_ << std::endl;

            auto tree_height = fmm::fmm_tree_height(n_centers);
            if (tree_height != last_tree_height) {
                solver = std::make_unique<rbf_solver>(model_, tree_height, bbox_);
                res_eval = std::make_unique<rbf_evaluator<>>(model_, tree_height, bbox_);
                last_tree_height = tree_height;
            }

            geometry::points3d center_points = points_(centers, Eigen::all);

            solver->set_points(center_points);
            center_weights =
                    solver->solve(values(centers, Eigen::all), absolute_tolerance, max_iter, center_weights);

            if (n_centers == n_points_) {
                break;
            }

            // Evaluate residuals at remaining points.

            auto c_centers = complementary_indices(centers);
            geometry::points3d c_center_points = points_(c_centers, Eigen::all);

            res_eval->set_source_points(center_points);
            res_eval->set_weights(center_weights);

            auto c_values_fit = res_eval->evaluate(c_center_points);
            valuesd c_values = values(c_centers, Eigen::all);
            std::vector<double> c_residuals(c_centers.size());
            valuesd::Map(c_residuals.data(), static_cast<index_t>(c_centers.size())) =
                    (c_values_fit - c_values).cwiseAbs();

            // Sort remaining points by their residuals.

            common::zip_sort(c_centers.begin(), c_centers.end(), c_residuals.begin(), c_residuals.end(),
                             [](const auto& a, const auto& b) { return a.second < b.second; });

            // Count points with residuals larger than absolute_tolerance.

            auto lb = std::lower_bound(c_residuals.begin(), c_residuals.end(), absolute_tolerance);
            auto n_points_need_fitting = static_cast<index_t>(std::distance(lb, c_residuals.end()));
            std::cout << "Number of points to fit: " << n_points_need_fitting << std::endl;

            if (n_points_need_fitting == 0) {
                break;
            }

            // Append points with the largest residuals.

            auto n_last_centers = n_centers;

            std::vector<index_t> indices(centers);
            std::copy(c_centers.rbegin(), c_centers.rend(), std::back_inserter(indices));
            point_cloud::distance_filter filter(points_, filtering_distance, indices);
            std::unordered_set<index_t> filtered_indices(filter.filtered_indices().begin(),
                                                         filter.filtered_indices().end());

            for (auto it = c_centers.rbegin(); it != c_centers.rbegin() + n_points_need_fitting; ++it) {
                if (filtered_indices.contains(*it)) {
                    centers.push_back(*it);
                }
            }

            n_centers = static_cast<index_t>(centers.size());

            auto last_center_weights = center_weights;
            center_weights = valuesd::Zero(n_centers + n_poly_basis_);
            center_weights.head(n_last_centers) = last_center_weights.head(n_last_centers);
            center_weights.tail(n_poly_basis_) = last_center_weights.tail(n_poly_basis_);

            filtering_distance *= 0.5;
        }

        return {std::move(centers), std::move(center_weights)};
    }

    std::vector<index_t> rbf_incremental_fitter::complementary_indices(
            const std::vector<index_t>& indices) const {
        std::vector<index_t> c_idcs(n_points_ - indices.size());

        auto universe = boost::irange<index_t>(index_t{0}, n_points_);
        auto idcs = indices;
        std::sort(idcs.begin(), idcs.end());
        std::set_difference(universe.begin(), universe.end(), idcs.begin(), idcs.end(), c_idcs.begin());

        return c_idcs;
    }
}