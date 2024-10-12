#include "ann.h"


// Return the Euclidean distance between two points
template <typename datatype>
float ANN<datatype>::calculateDistance(const std::vector<datatype>& a, const std::vector<datatype>& b){
    float distance = .0;
    for(std::size_t i = 0; i < a.size(); i++){
        distance += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return std::sqrt(distance);
}

template <typename datatype>
ANN<datatype>::ANN(){}

template <typename datatype>
void ANN<datatype>::pruneSet(std::set<std::vector<datatype>> myset,int k){
    
    if (myset.size() <= static_cast<size_t>(k)) {
        return;
    }
    auto start = myset.begin();
    std::advance(start,myset.size()-k);
    myset.erase(myset.begin(),start);
    return;
    
}

template <typename datatype>
std::set<std::vector<datatype>> ANN<datatype>::neighbourNodes(std::vector<datatype> point){

}

// Greedy search algorithm to find the nearest neighbours
template <typename datatype>
std::vector<std::vector<datatype>> ANN<datatype>::greedySearch(const std::vector<datatype>& start, const std::vector<datatype>& query, int k, int upper_limit){
    // Create a lambda that will compare the 
    // distance between two points and use it to sort the set
    auto compare = [&](std::vector<datatype>& a, std::vector<datatype>& b){
        return this->calculateDistance(a, query) < this->calculateDistance(b, query);   
    };

    std::set<std::vector<datatype>, decltype(compare)> visited(compare);
    std::set<std::vector<datatype>, decltype(compare)> nns(compare);
    nns.insert(start);

    while (true) {
        std::set<std::vector<datatype>, decltype(compare)> difference(compare);

        // Calculate the difference between nns and visited
        std::set_difference(nns.begin(), nns.end(), visited.begin(), visited.end(),
                            std::back_inserter(difference));

        // If the difference is empty, break out of the loop
        if (difference.empty()) {
            break;
        }

        // Get the index of the closest point
        std::vector<datatype> closest_point = *difference.begin();
        

        // TODO Make Neighbors Function
        // neighbourNodes(closest_point)
        std::set<std::vector<datatype>> neighbours;
        

        // Update nns set with neighbours of closest_point
        for(auto i=neighbours.begin;i++;i<=neighbours.end()){
            nns.insert(i);
        }

        //Update Visited set with closest_point

        visited.insert(closest_point);

        if(nns.size()>upper_limit){
            //Update nns to retain upper_limit closest points
            ANN::pruneSet(nns,upper_limit);
        }
        
    }

    // Return k closest points to Xq
    ANN::pruneSet(nns,k);
    return nns;
}

template <typename datatype>
void ANN<datatype>::robustPrune(std::vector<datatype> point, std::set<std::vector<datatype>> candidate_set,int alpha,int degree_bound/*,graph G*/){
    std::set<std::vector<datatype>> neighbours = ANN::neighbourNodes(point);

    for(auto i=neighbours.begin;i++;i<=neighbours.end()){
        if(ANN::calculateDistance(point,i) < alpha){
            candidate_set.insert(i);
        }
    }

    // Remove point p from candidate_set
    candidate_set.erase(point);

    // TODO: Erase Neighbours of p from Graph

    while(true){
        if(candidate_set.size() ==0 ){
            break;
        }
        std::set<std::vector<datatype>> difference
    }
}