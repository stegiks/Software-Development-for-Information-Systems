#ifndef ANN_H
#define ANN_H

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include "utils_ann.h"

template <typename datatype>
class ANN{
private:

    static Graph G;

    std::vector<std::vector<datatype>> base_vector;

    float calculateDistance(const std::vector<datatype>& a, const std::vector<datatype>& b);
    
    void pruneSet(std::set<std::vector<datatype>> myset, int k);

    std::set<std::vector<datatype>> ANN::neighbourNodes(std::vector<datatype> point);
public:
    ANN();
    
    std::vector<std::vector<datatype>> greedySearch(const std::vector<datatype>& start_node, const std::vector<datatype>& query_node, int k, int upper_limit);

    void ANN::robustPrune(std::vector<datatype> point,std::set<std::vector<datatype>> candidate_set,int alpha,int degree_bound/*,graph G*/);
};

#endif // ann.h