#include "ann.h"

// Prune the set to retain only the k closest points
template <typename datatype>
template <typename Compare>
void ANN<datatype>::pruneSet(std::set<std::vector<datatype>, Compare>& myset, int k){

    if(myset.size() <= static_cast<std::size_t>(k))
        return;

    // Erase all elements after the kth element
    auto start = myset.begin();
    std::advance(start, k);
    myset.erase(start, myset.end());
}

// Return the neighbors of a point
template <typename datatype>
void ANN<datatype>::neighbourNodes(std::vector<datatype> point, std::vector<std::vector<datatype>>& neighbours){

    // Retrieve the node index for the point
    int node = this->point_to_node_map[point];
    std::vector<int> neighbour_indices = this->G.getNeighbours(node);

    // Convert the neighbor indices back to points
    for(int index : neighbour_indices)
        neighbours.push_back(this->node_to_point_map[index]);

}

// Constructor for building a random graph
template <typename datatype>
ANN<datatype>::ANN(const std::vector<std::vector<datatype>>& points) : G(points.size()){
    int node_index = 0;

    // Populate the node_to_point_map and point_to_node_map
    for(const auto& point : points){
        this->node_to_point_map.push_back(point);
        this->point_to_node_map[point] = node_index;
        node_index++;
    }
}

// Constructor for building a fixed graph used for testing
template <typename datatype>
ANN<datatype>::ANN(const std::vector<std::vector<datatype>>& points, const std::vector<std::vector<int>>& edges) : G(edges){
    int node_index = 0;

    // Populate the node_to_point_map and point_to_node_map
    for(const auto& point : points){
        this->node_to_point_map.push_back(point);
        this->point_to_node_map[point] = node_index;
        node_index++;
    }
}

template<typename datatype>
bool ANN<datatype>::checkErrorsGreedy(const std::vector<datatype>& start, const std::vector<datatype>& query, int k, int upper_limit){
    if(this->node_to_point_map.empty()){
        std::cerr << "Error : Graph is empty" << std::endl;
        return true;
    }

    if(query.size() == 0){
        std::cerr << "Error : Query point is empty" << std::endl;
        return true;
    }

    auto iterator = this->point_to_node_map.find(start);

    if(iterator == this->point_to_node_map.end()){
        std::cerr << "Error : Start node not found in the graph" << std::endl;
        return true;
    }
    
    if(upper_limit < k){
        std::cerr << "Error : Upper limit cannot be less than k" << std::endl;
        return true;
    }

    if(query.size() != start.size()){
        std::cerr << "Error : Query point and start point have different dimensions" << std::endl;
        return true;
    }

    return false;
}

// Greedy search algorithm to find the nearest neighbours
template <typename datatype>
std::set<std::vector<datatype>> ANN<datatype>::greedySearch(const std::vector<datatype>& start, const std::vector<datatype>& query, int k, int upper_limit){
    
    // Error handling. Maybe Vamana can handle these cases
    if(this->checkErrorsGreedy(start, query, k, upper_limit)){
        return {};
    }

    // Comparator object to compare points based on the distance from the query point
    CompareVectors<datatype> compare(query);

    std::set<std::vector<datatype>, CompareVectors<datatype>> visited(compare);
    std::set<std::vector<datatype>, CompareVectors<datatype>> nns(compare);
    nns.insert(start);

    while(true){
        std::set<std::vector<datatype>, CompareVectors<datatype>> difference(compare);
        
        std::set_difference(nns.begin(), nns.end(), visited.begin(), visited.end(),
                    std::inserter(difference, difference.end()), compare);

        // If the difference is empty, break out of the loop
        if(difference.empty())
            break;

        // Get the index of the closest point
        std::vector<datatype> closest_point = *(difference.begin());
 
        // Get the neighbors of the closest point
        std::vector<std::vector<datatype>> neighbours;
        this->neighbourNodes(closest_point, neighbours);

        // Update nns set with neighbours of closest_point
        for(const auto& neighbour : neighbours)
            nns.insert(neighbour);

        // Update visited set with closest_point
        visited.insert(closest_point);

        // Update nns to retain upper_limit closest points
        if(nns.size() > static_cast<std::size_t>(upper_limit))
            this->pruneSet(nns, upper_limit);
        
    }


    // Return k closest points to Xq, using regular set
    this->pruneSet(nns, k);
    std::set<std::vector<datatype>> nns_copy(nns.begin(), nns.end());
    nns.clear();
    return nns_copy;
}

template <typename datatype>
void ANN<datatype>::robustPrune(std::vector<datatype> point, std::set<std::vector<datatype>> candidate_set, float alpha, int degree_bound){
    std::vector<std::vector<datatype>> neighbours;
    this->neighbourNodes(point, neighbours);

    for(const auto& neighbour : neighbours){
        candidate_set.insert(neighbour);
    }

    // Remove point p from candidate_set and prune all its neighbours
    candidate_set.erase(point);
    this->G.removeNeighbours(this->point_to_node_map[point]);

    while(true){

        if(candidate_set.size() == 0){
            break;
        }
        std::vector<datatype> closest_point = *(candidate_set.begin());
        candidate_set.erase(closest_point);
        this->G.addEdge(this->point_to_node_map[point], this->point_to_node_map[closest_point]);

        if(this->G.countNeighbours(this->point_to_node_map[point]) > degree_bound){
            break;
        }


        std::set<std::vector<datatype>> temp_set(candidate_set);
        for(const auto& element : candidate_set){
            std::cout << "Comparing " << alpha * float(calculateDistance(closest_point, element)) << " and " << float(calculateDistance(element, point)) << std::endl;
            if(alpha * float(calculateDistance(closest_point, element)) <= float(calculateDistance(element, point))){
                temp_set.erase(element);
            }
        }
        candidate_set.clear();
        candidate_set = temp_set;
    }
}

template <typename datatype>
bool ANN<datatype>::checkGraph(std::vector<std::vector<int>> edges){
    this->G.printGraph();
    return this->G.checkSimilarity(edges);
}

// Explicit Instantiation of ANN class for datatype int, float and unsigned char
template class ANN<int>;
template class ANN<float>;
template class ANN<long>;
template class ANN<unsigned char>;