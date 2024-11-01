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
#include <random>
#include <optional>
#include <chrono>

template <class datatype>
class ANN{
private:
    Graph* G;
    std::vector<std::vector<datatype>> node_to_point_map;
    std::unordered_map<std::vector<datatype>, int, VectorHash<datatype>> point_to_node_map;
    std::optional<std::vector<datatype>> cached_medoid;

    template <typename Compare>
    void pruneSet(std::set<std::vector<datatype>, Compare>& myset, std::set<std::vector<datatype>, Compare>& diff, int k);

    bool checkErrorsGreedy(const std::vector<datatype>& start, const std::vector<datatype>& query, int k, int upper_limit);
    bool checkErrorsRobust(std::vector<datatype> point, const float alpha, const int degree_bound);
    void calculateMedoid();

    friend class ANNTest;  // Used for testing private methods

public:
    ANN(const std::vector<std::vector<datatype>>& points);
    ANN(const std::vector<std::vector<datatype>>& points, const std::vector<std::unordered_set<int>>& edges);
    ~ANN();
    bool checkGraph(std::vector<std::unordered_set<int>> edges);
    bool checkNeighbour(std::vector<datatype> a, std::vector<datatype> b);
    const std::vector<datatype>& getMedoid();

    template <typename Compare, typename Vechash>
    void greedySearch(const std::vector<datatype>& start_node, const std::vector<datatype>& query_node, int k, int upper_limit, std::set<std::vector<datatype>, Compare>& NNS, std::unordered_set<std::vector<datatype>, Vechash>& Visited, CompareVectors<datatype>& compare);

    template <typename Compare>
    void robustPrune(std::vector<datatype> point, std::set<std::vector<datatype>, Compare>& candidate_set, const float alpha, const int degree_bound);
    void Vamana(float alpha,int L,int R);

    void neighbourNodes(std::vector<datatype>& point, std::vector<std::vector<datatype>>& neighbours);
    int countNeighbours(int node);
};
#endif // ann.h