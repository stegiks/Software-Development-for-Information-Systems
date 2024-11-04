#ifndef UTILS_ANN_H
#define UTILS_ANN_H

#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#define FNV_BASIS 0x811c9dc5
#define FNV_PRIME 0x01000193

// FVN-1 Hash Function.
template <typename datatype>
struct VectorHash{
    std::size_t operator()(const std::vector<datatype>& vec) const{
        std::size_t hash_seed = FNV_BASIS;
        for(auto& coordinate : vec){
            std::hash<datatype> hasher;
            hash_seed *= FNV_PRIME;
            hash_seed ^= hasher(coordinate);
        }
        return hash_seed;
    }
};

// Utility function to calculate the Euclidean distance between two vectors
template <typename datatype>
float calculateDistance(const std::vector<datatype>& a, const std::vector<datatype>& b){
    float distance = .0;
    for(std::size_t i = 0; i < a.size(); i++){
        distance += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return std::sqrt(distance);
}


// Comparator class for comparing indices based on the distance from a query point
template <typename datatype>
class CompareVectors{
public:
    const std::vector<std::vector<datatype>>& m_node_to_point_map;  // Map from index to vector
    const std::vector<datatype>& m_compare_vector;  // The query point to compare distances to

    // Constructor now takes node-to-point map and a comparison vector
    CompareVectors(const std::vector<std::vector<datatype>>& node_to_point_map, 
                   const std::vector<datatype>& compare_vector)
        : m_node_to_point_map(node_to_point_map), m_compare_vector(compare_vector) {
            if(m_node_to_point_map.empty()){
                throw std::invalid_argument("Node to point map is empty");
            }
            
            if(m_compare_vector.size() != m_node_to_point_map[0].size()){
                throw std::invalid_argument("Query vector size does not match the data vector size");
            }
            
        }

    // Operator() compares the distances of points at indices a and b to the comparison vector
    bool operator()(int a, int b) const {
        return calculateDistance(m_node_to_point_map[a], m_compare_vector) 
             < calculateDistance(m_node_to_point_map[b], m_compare_vector); 
    }
};
#endif // ann_utils.h