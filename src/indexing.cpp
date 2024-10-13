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

// Prune the set to retain only the k closest points
template <typename datatype>
void ANN<datatype>::pruneSet(std::set<std::vector<datatype>> myset, int k){
    
    if(myset.size() <= static_cast<size_t>(k))
        return;

    // Erase all elements after the kth element
    auto start = myset.begin();
    std::advance(start, k);
    myset.erase(start, myset.end());
    return;
    
}

// Return the neighbors of a point
template <typename datatype>
std::set<std::vector<datatype>> ANN<datatype>::neighbourNodes(std::vector<datatype> point){
    std::set<std::vector<datatype>> neighbours;
    
    // Retrieve the node index for the point
    int node = point_to_node_map[point];
    std::vector<int> neighbour_indices = graph.getNeighbours(node);
    
    // Convert the neighbor indices back to points
    for(int index : neighbour_indices){
        neighbours.insert(node_to_point_map[index]);
    }
    
    return neighbours;
}


template <typename datatype>
ANN<datatype>::ANN(const std::vector<std::vector<datatype>>& points) {
    // Initialize the graph with the number of nodes equal to the number of points
    Graph graph(points.size());
    int node_index = 0;

    // Populate the node_to_point_map and point_to_node_map
    for(const auto& point : points){
        node_to_point_map.push_back(point);
        node_index++;
        point_to_node_map[point] = node_index;
    }
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
        
        std::set_difference(nns.begin(), nns.end(), visited.begin(), visited.end(),
                    std::inserter(difference, difference.end()));

        // If the difference is empty, break out of the loop
        if (difference.empty()) {
            break;
        }

        // Get the index of the closest point
        std::vector<datatype> closest_point = *difference.begin();
        
        // Get the neighbors of the closest point
        std::set<std::vector<datatype>> neighbours = ANN::neighbourNodes(closest_point);
        

        // Update nns set with neighbours of closest_point
        for(auto i=neighbours.begin; i++; i<=neighbours.end()){
            nns.insert(*i);
        }

        //Update Visited set with closest_point
        visited.insert(closest_point);

        if(nns.size() > upper_limit){
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
        candidate_set.insert(*i);
    }

    // Remove point p from candidate_set and prune all its neighbours
    candidate_set.erase(point);
    this->G.removeNeighbours(this->point_to_node_map[point]);


    // Reorder the candidate_set based on the distance from the query point
    std::vector<std::vector<datatype>> candidate_vector(candidate_set.begin(), candidate_set.end());

    auto compare = [&](const std::vector<datatype>& a, const std::vector<datatype>& b) {
        return this->calculateDistance(a, point) < this->calculateDistance(b, point);
    };

    // Sort the candidate_vector using the custom compare function
    std::sort(candidate_vector.begin(), candidate_vector.end(), compare);

    // Clear the candidate_set and insert the sorted candidates back
    candidate_set.clear();
    candidate_set.insert(candidate_vector.begin(), candidate_vector.end());

    while(true){
        if(candidate_set.size() ==0 ){
            break;
        }
        std::vector<datatype> closest_point = *candidate_set.begin();
        this->G.addEdge(this->point_to_node_map[point],this->point_to_node_map[closest_point]);

        if(this->G.countNeighbours(this->point_to_node_map[point]) > degree_bound){
            break;
        }
        for(auto i=candidate_set.begin;i++;i<=candidate_set.end()){
            if(this->calculateDistance(*i,point) <= alpha*this->calculateDistance(closest_point,point)){
                candidate_set.erase(*i);
            }
        }
    }
}