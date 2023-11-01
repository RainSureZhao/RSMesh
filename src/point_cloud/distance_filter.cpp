//
// Created by 赵润朔 on 2023/11/1.
//

#include "point_cloud/distance_filter.h"
#include "point_cloud/kdtree.h"
#include <set>
#include <tuple>

namespace rsmesh {
    namespace point_cloud {
        
        distance_filter::distance_filter(const geometry::points3d &points, double distance_threshold) : 
            n_points_(static_cast<index_t>(points.rows())) {
            if (distance_threshold <= 0.0) {
                throw std::invalid_argument("distance_threshold must be greater than 0.0");
            }
            
            kdtree tree(points, true);
            
            std::vector<index_t> nn_indices;
            std::vector<double> nn_distances;
            std::set<index_t> indices_to_remove;
            
            for (index_t i = 0; i < n_points_; i ++) {
                if(indices_to_remove.find(i) != indices_to_remove.end()) continue;
                
                geometry::points3d p = points.row(i);
                std::tie(nn_indices, nn_distances) = tree.radius_search(p, distance_threshold);
                
                for(auto j : nn_indices) {
                    if(j != i) {
                        indices_to_remove.insert(j);
                    }
                }
            }
            
            for(index_t i = 0; i < n_points_; i ++) {
                if(indices_to_remove.find(i) == indices_to_remove.end()) {
                    filterer_indices_.push_back(i);
                }
            }
        }
    } // rsmesh
} // point_cloud