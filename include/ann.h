#ifndef ANN_H
#define ANN_H

#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include "utils_ann.h"

// ! CHANGE THE OPERATIONS MAYBE 
template <typename datatype>
struct VectorHash{
    std::size_t operator()(const std::vector<datatype>& vec) const{
        std::size_t seed = vec.size();
        for(auto& coordinate : vec){
            std::hash<datatype> hasher;
            seed ^= hasher(coordinate) + 0x06101425 + (seed << 6) + (seed >> 3);
        }
        return seed;
    }
};

template <typename datatype>
class ANN{
private:

    static Graph G;

    std::vector<std::vector<datatype>> node_to_point_map;
    std::unordered_map<std::vector<datatype>, int, VectorHash> point_to_node_map;

    float calculateDistance(const std::vector<datatype>& a, const std::vector<datatype>& b);
    void pruneSet(std::set<std::vector<datatype>> myset, int k);
    std::set<std::vector<datatype>> ANN::neighbourNodes(std::vector<datatype> point);

public:
    ANN(const std::vector<std::vector<datatype>>& points);
    
    std::vector<std::vector<datatype>> greedySearch(const std::vector<datatype>& start_node, const std::vector<datatype>& query_node, int k, int upper_limit);
    void ANN::robustPrune(std::vector<datatype> point,std::set<std::vector<datatype>> candidate_set,int alpha,int degree_bound/*,graph G*/);
};

#endif // ann.h