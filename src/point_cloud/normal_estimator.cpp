//
// Created by RainSure on 2023/11/3.
//

#include "point_cloud/normal_estimator.h"

#include <stdexcept>
#include <tuple>
#include <numeric>
#include <queue>
#include <iostream>

#include "common/eigen_utility.h"
#include "point_cloud/plane_estimator.h"
#include "geometry/bbox3d.h"

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
                     auto tmp = points_(nn_indices, Eigen::all);
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
        
        class weighted_pair {
        public:
            weighted_pair(index_t first, index_t second, double weight) 
             : first_(first), second_(second), weight_(weight) {}
             
             bool operator<(const weighted_pair& rhs) const {
                return weight_ < rhs.weight_;
             }
            
             [[nodiscard]] index_t first() const {return first_;}
             
             [[nodiscard]] index_t second() const {return second_;}
             
             [[nodiscard]] double weight() const {return weight_;}
             
             
        private:
            index_t first_;
            index_t second_;
            double weight_;
        };


        geometry::vectors3d normal_estimator::orient_closed_surface(rsmesh::index_t k) {
            if(n_points_ > 0 and normals_.rows() == 0) {
                throw std::runtime_error("Normals have not been estimated yet.");
            }
            
            auto bbox = geometry::bbox3d::from_points(points_);
            auto center = bbox.center();
            geometry::point3d p_outer(center(0), bbox.min()(1) - 1.0, center(2));
            
            std::vector<bool> oriented(n_points_, false);
            
            for(index_t i = 0; i < n_points_; i ++) {
                if(normals_.row(i).isZero()) {
                    oriented.at(i) = true;
                }
            }
            
            std::vector<index_t> indices(n_points_);
            std::iota(begin(indices), end(indices), 0);
            {
                std::vector<double> distances(n_points_);
                for(index_t i = 0; i < n_points_; i ++) {
                    auto p = points_.row(i);
                    distances.at(i) = (p_outer - p).norm();
                }
                
                std::sort(begin(indices), end(indices), [&](int i, int j) {
                    return distances.at(i) < distances.at(j);
                });
            }
            
            auto indices_it = begin(indices);
            
            std::priority_queue<weighted_pair> queue;
            std::vector<index_t> nn_indices;
            std::vector<double> nn_distances;
            
            index_t n_connected_components = 0;
            while(std::find(begin(oriented), end(oriented), false) != end(oriented)) {
                while(oriented.at(*indices_it)) {
                    indices_it ++;
                }
                
                auto i_closest = *indices_it;
                auto p_closest = points_.row(i_closest);
                if(normals_.row(i_closest).dot(p_outer - p_closest) < 0.0) {
                    normals_.row(i_closest) *= -1.0;
                }
                oriented.at(i_closest) = true;
                
                std::tie(nn_indices, nn_distances) = tree_.knn_search(p_closest, k);
                for(auto j : nn_indices) {
                    if(oriented.at(j)) {
                        continue;
                    }
                    
                    auto weight = std::abs(normals_.row(i_closest).dot(normals_.row(j))) 
                            / (p_closest - points_.row(j)).norm();
                    queue.emplace(i_closest, j, weight);
                }
                
                while(!queue.empty()) {
                    auto pair = queue.top();
                    queue.pop();
                    
                    auto i = pair.first();
                    auto j = pair.second();
                    if(oriented.at(j)) {
                        continue;
                    }
                    
                    if(normals_.row(i).dot(normals_.row(j)) < 0.0) {
                        normals_.row(j) *= -1.0;
                    }
                    oriented.at(j) = true;
                    
                    auto p = points_.row(j);
                    std::tie(nn_indices, nn_distances) = tree_.knn_search(p, k);
                    for(auto kk : nn_indices) {
                        if(oriented.at(kk)) {
                            continue;
                        }
                        
                        auto weight = std::abs(normals_.row(j).dot(normals_.row(kk)))
                                / (p - points_.row(kk)).norm();
                        
                        queue.emplace(j, kk, weight);
                    }
                }
                
                n_connected_components ++;
            }
            
            std::cout << "Number of connected components: " << n_connected_components << std::endl;
            
            return normals_;
        }
        
    } // rsmesh
} // point_cloud