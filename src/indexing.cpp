#include "ann.h"

// Return the Euclidean distance between two points
template <typename point>
float ANN::calculateDistance(const std::vector<point>& a, const std::vector<point>& b){
    float distance = .0;
    for(std::size_t i = 0; i < a.size(); i++){
        distance += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return std::sqrt(distance);
}


ANN::ANN(){}

// Greedy search algorithm to find the nearest neighbours
template <typename point>
std::vector<std::vector<point>> ANN::greedySearch(const std::vector<point>& start, const std::vector<point>& query, int k, int upper_limit){

    std::set<point> visited;
    std::vector<point> nns = {start};
    
    // Make a heap to get the nearest neighbour.
    // Create a lambda that will compare the distance between two points.
    auto compare = [&](std::vector<point>& a, std::vector<point>& b){
        return this->calculateDistance(a, query) > this->calculateDistance(b, query);
    };

    std::make_heap(nns.begin(), nns.end(), compare);

    // Loop until nns set is the same as visited set
    while(std::set_difference(nns.begin(), nns.end(),visited.begin(), visited.end()) != 0){
        // TODO : Implement the greedy search algorithm
    }
}