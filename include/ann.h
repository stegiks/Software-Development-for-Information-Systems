#ifndef ANN_H
#define ANN_H

#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <algorithm>
#include <iterator> 
#include "graph.h"
#include "utils_ann.h"

template <class datatype>
class ANN{
private:
    Graph* G;
    std::vector<std::vector<datatype>> node_to_point_map;
    std::unordered_map<std::vector<datatype>, int, VectorHash<datatype>> point_to_node_map;

    template <typename Compare>
    void pruneSet(std::set<std::vector<datatype>, Compare>& myset, int k);
    void neighbourNodes(std::vector<datatype> point, std::vector<std::vector<datatype>>& neighbours);

    bool checkErrorsGreedy(const std::vector<datatype>& start, const std::vector<datatype>& query, int k, int upper_limit);
    bool checkErrorsRobust(std::vector<datatype> point, const float alpha, const int degree_bound);

public:
    ANN(const std::vector<std::vector<datatype>>& points);
    ANN(const std::vector<std::vector<datatype>>& points, const std::vector<std::unordered_set<int>>& edges);

    std::set<std::vector<datatype>> greedySearch(const std::vector<datatype>& start_node, const std::vector<datatype>& query_node, int k, int upper_limit);
    template <typename Compare>
    void robustPrune(std::vector<datatype> point, std::set<std::vector<datatype>, Compare>& candidate_set, const float alpha, const int degree_bound);
    bool checkGraph(std::vector<std::unordered_set<int>> edges);
};

#endif // ann.h