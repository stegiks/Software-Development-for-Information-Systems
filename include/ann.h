#ifndef ANN_H
#define ANN_H

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

class ANN{
private:

    template <typename point>
    float calculateDistance(const std::vector<point>& a, const std::vector<point>& b);

public:
    ANN();

    template <typename point>
    std::vector<std::vector<point>> greedySearch(const std::vector<point>& start_node, const std::vector<point>& query_node, int k, int upper_limit);
};

#endif // ann.h