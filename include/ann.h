#ifndef ANN_H
#define ANN_H


#define FILTERED true
#define UNFILTERED false

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
    std::unordered_map<std::vector<datatype>, int, VectorHash<datatype>> point_to_node_map;
    std::unordered_map<float, std::vector<int>> filter_to_node_map;
    std::unordered_map<float, int> filter_to_start_node;
    std::optional<int> cached_medoid;

    template <typename Compare>
    void pruneSet(std::set<int, Compare>&,std::set<int, Compare> &, int k);

    bool checkErrorsGreedy(const int &start, int k, int upper_limit);
    bool checkErrorsRobust(const int &point, const float alpha, const int degree_bound);
    void calculateMedoid();

    void filteredPruning();
public:
    std::vector<std::vector<datatype>> node_to_point_map;
    std::vector<float> node_to_filter_map;                  // Filter values for each node

    ANN(const std::vector<std::vector<datatype>>& points);
    ANN(const std::vector<std::vector<datatype>>& points, const std::vector<std::unordered_set<int>>& edges);
    ANN(const std::vector<std::vector<datatype>>& points, size_t reg);
    ANN(const std::vector<std::vector<datatype>>& points, const std::vector<float>& filters);
    ANN(const std::vector<std::vector<datatype>>& points, const std::vector<std::unordered_set<int>>& edges, const std::vector<float>& filters);

    ~ANN();
    bool checkGraph(std::vector<std::unordered_set<int>> edges);
    bool checkNeighbour(int a, int b);
    const int& getMedoid();

    // For testing
    bool checkFilteredFindMedoid(std::size_t num_of_filters);
    int getStartNode(float filter);
    void filteredFindMedoid(int tau);

    // Fill filter_to_start_node for testing
    void fillFilterToStartNode(std::unordered_map<float, int>& filter_to_start_node);

    template <typename Compare>
    void greedySearch(const int & start_node, int k, int upper_limit, std::set<int, Compare>& NNS, std::unordered_set<int>& Visited, CompareVectors<datatype>& compare);
    template <typename Compare>
    void filteredGreedySearch(const int & start_node, int k, int upper_limit,const float & filter, std::set<int, Compare>& NNS, std::unordered_set<int>& Visited, CompareVectors<datatype>& compare);

    
    template <typename Compare>
    void robustPrune(const int & point, std::set<int, Compare>& candidate_set, const float alpha, const int degree_bound, bool filtered);
    
    void Vamana(float alpha, int L, int R);
    void filteredVamana(float alpha, int L, int R, int tau);
    void stitchedVamana(float alpha, int L_small, int R_small, int R_stitched);

    void neighbourNodes(const int& point, std::vector<int>& neighbours);
    int countNeighbours(int node);

    void saveGraph(const std::string &file_path);
    void loadGraph(const std::string &file_path);
    void printGraph();
};
#endif // ann.h