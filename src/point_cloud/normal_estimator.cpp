//
// Created by 赵润朔 on 2023/11/3.
//

#include "point_cloud/normal_estimator.h"

#include <stdexcept>
#include <tuple>

#include "common/eigen_utility.h"
#include "point_cloud/plane_estimator.h"

namespace rsmesh {
    namespace point_cloud {
        normal_estimator::normal_estimator(const geometry::points3d &points) : 
        n_points_(static_cast<index_t>(points.rows()))
        , points_(points)
        , tree_(points, true){}

        normal_estimator &normal_estimator::estimate_with_knn(rsmesh::index_t k, double plane_factor_threshold) {
            return estimate_with_knn(std::vector<index_t>{k}, plane_factor_threshold);
        }

        normal_estimator &normal_estimator::estimate_with_radius(double radius, double plane_factor_threshold) {
            normals_ = geometry::vectors3d::Zero(n_points_, 3);

            {
                std::vector<index_t> nn_indices;
                std::vector<double> nn_distances;
                
                for(index_t i = 0; i < n_points_; i ++) {
                    auto p = points_.row(i);
                    std::tie(nn_indices, nn_distances) = tree_.radius_search(p, radius);
                    if(nn_indices.size() < 3) {
                        continue;
                    }
                    
                    plane_estimator est(points_(nn_indices, Eigen::all));
                    
                    if(est.plane_factor() >= plane_factor_threshold) {
                        normals_.row(i) = est.plane_normal();
                    }
                }
            }
            
            return *this;
        }
        
        normal_estimator& normal_estimator::estimate_with_knn(const std::vector<index_t> &ks,
                                                              double plane_factor_threshold) {
            normals_ = geometry::vectors3d::Zero(n_points_, 3);
            
            std::vector<index_t> nn_indices;
            std::vector<double> nn_distances;
            
            std::vector<index_t> ks_sorted(ks);
            std::sort(ks_sorted.rbegin(), ks_sorted.rend());
            auto k_max = ks_sorted.front();
            
            std::vector<double> plane_factors;
            std::vector<geometry::vector3d> plane_normals;
            
            for(index_t i = 0; i < n_points_; i ++) {
                auto p = points_.row(i);
                
                 std::tie(nn_indices, nn_distances) =  tree_.knn_search(p, k_max);
                 
                 plane_factors.clear();
                 plane_normals.clear();
                 
                 for(auto k : ks_sorted) {
                     nn_indices.resize(k);
                     plane_estimator est(points_(nn_indices, Eigen::all));
                     plane_factors.push_back(est.plane_factor());
                     plane_normals.push_back(est.plane_normal());
                 }
                 
                 auto best = std::distance(plane_factors.begin(), 
                                           std::max_element(plane_factors.begin(), plane_factors.end()));
                 
                 if (plane_factors.at(best) >= plane_factor_threshold) {
                     normals_.row(i) = plane_normals.at(best);
                 }
            }
            return *this;
        }

        geometry::vectors3d normal_estimator::orient_by_outward_vector(const geometry::vector3d &v) { 
            if(n_points_ > 0 and normals_.rows() == 0) {
                throw std::runtime_error("Normals have not been estimated yet.");
            }
            
            for(index_t i = 0; i < n_points_; i ++) {
                auto n = normals_.row(i);
                if(n.dot(v) < 0.0) {
                    n = -n;
                }
            }
            
            return normals_;
        }
        
    } // rsmesh
} // point_cloud