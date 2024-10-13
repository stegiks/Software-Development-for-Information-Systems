#ifndef ANN_H
#define ANN_H

#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include "utils_ann.h"
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

template <typename datatype>
class ANN{
private:

    static Graph G;

    std::vector<std::vector<datatype>> node_to_point_map;
    std::unordered_map<std::vector<datatype>, int, VectorHash<datatype>> point_to_node_map;

    float calculateDistance(const std::vector<datatype>& a, const std::vector<datatype>& b);
    void pruneSet(std::set<std::vector<datatype>> myset, int k);
    std::set<std::vector<datatype>> ANN::neighbourNodes(std::vector<datatype> point);

public:
    ANN(const std::vector<std::vector<datatype>>& points);
    
    std::vector<std::vector<datatype>> greedySearch(const std::vector<datatype>& start_node, const std::vector<datatype>& query_node, int k, int upper_limit);
    void ANN::robustPrune(std::vector<datatype> point,std::set<std::vector<datatype>> candidate_set,int alpha,int degree_bound/*,graph G*/);
};

#endif // ann.h