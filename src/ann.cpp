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
    std::vector<int> neighbour_indices = this->G->getNeighbours(node);

    // Convert the neighbor indices back to points
    for(int index : neighbour_indices)
        neighbours.push_back(this->node_to_point_map[index]);

}

// Constructor for building a random graph
template <typename datatype>
ANN<datatype>::ANN(const std::vector<std::vector<datatype>>& points){
    int node_index = 0;

    this->G = new Graph(points.size());  // Call the Graph constructor with number of points
    // Populate the node_to_point_map and point_to_node_map
    for(const auto& point : points){
        this->node_to_point_map.push_back(point);
        this->point_to_node_map[point] = node_index;
        node_index++;
    }
}

template <typename datatype>
ANN<datatype>::ANN(const std::vector<std::vector<datatype>>& points, const std::vector<std::vector<int>>& edges) {
    std::size_t num_nodes = points.size();

    int node_index = 0;

    for (const auto& point : points) {
        this->node_to_point_map.push_back(point);
        this->point_to_node_map[point] = node_index;
        node_index++;
    }

    if (edges.empty() || edges[0].empty() || edges.size() != num_nodes || edges[0].size() != num_nodes) {
        this->G = new Graph(num_nodes);  // Initialize graph with number of points
    } else {
        this->G = new Graph(edges);  // Initialize graph with edges
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

template <typename datatype>
bool ANN<datatype>::checkErrorsRobust(std::vector<datatype> point, const float alpha, const int degree_bound){

    if(this->node_to_point_map.empty()){
        std::cerr << "Error : Graph is empty" << std::endl;
        return true;
    }

    if(point.size() == 0){
        std::cerr << "Error : Point is empty" << std::endl;
        return true;
    }

    auto iterator = this->point_to_node_map.find(point);
    if(iterator == this->point_to_node_map.end()){
        std::cerr << "Error : Point not found in the graph" << std::endl;
        return true;
    }

    if(alpha < 1.0){
        std::cerr << "Error : Alpha cannot be less than 1" << std::endl;
        return true;
    }

    if(degree_bound < 0){
        std::cerr << "Error : Degree bound cannot be negative" << std::endl;
        return true;
    }

    return false;
}

// Greedy search algorithm to find the nearest neighbours
template <typename datatype>
std::set<std::vector<datatype>> ANN<datatype>::greedySearch(const std::vector<datatype>& start, const std::vector<datatype>& query, int k, int upper_limit){
    
    // Error handling
    if(this->checkErrorsGreedy(start, query, k, upper_limit))
        return {};

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
template <typename Compare>
void ANN<datatype>::robustPrune(std::vector<datatype> point, std::set<std::vector<datatype>, Compare>& candidate_set, const float alpha, const int degree_bound){
    
    // Error handling
    if(this->checkErrorsRobust(point, alpha, degree_bound))
        return;
    
    std::vector<std::vector<datatype>> neighbours;
    this->neighbourNodes(point, neighbours);

    for(const auto& neighbour : neighbours){
        candidate_set.insert(neighbour);
    }

    // Remove point p from candidate_set and prune all its neighbours
    candidate_set.erase(point);
    this->G->removeNeighbours(this->point_to_node_map[point]);

    while(true){

        if(candidate_set.size() == 0)
            break;

        std::vector<datatype> closest_point = *(candidate_set.begin());

        // Closest point would have been removed from candidate set in alpha comparison step anyway
        candidate_set.erase(closest_point);
        this->G->addEdge(this->point_to_node_map[point], this->point_to_node_map[closest_point]);

        if(this->G->countNeighbours(this->point_to_node_map[point]) == degree_bound)
            break;

        for(auto it = candidate_set.begin(); it != candidate_set.end();){
            const auto& element = *it;

            if(alpha * float(calculateDistance(closest_point, element)) <= float(calculateDistance(element, point)))
                it = candidate_set.erase(it);
            else
                it++;  // Only advance iterator if no deletion
        }
    }
}

template <typename datatype>
bool ANN<datatype>::checkGraph(std::vector<std::vector<int>> edges){
    this->G->printGraph();
    return this->G->checkSimilarity(edges);
}


template <typename datatype>
std::vector<datatype> ANN<datatype> :: getMedoid(){
    std::vector<datatype> medoid;

    // Get the point with the minimum sum of distances
    float min_sum = std::numeric_limits<float>::max();

    for(size_t i=0;i<this->node_to_point_map.size();i++){
        
        float sum = 0;
        for(size_t j=0;j<this->node_to_point_map.size();j++){
            sum += calculateDistance(this->node_to_point_map[i],this->node_to_point_map[j]);
        }

        if(sum < min_sum){
            min_sum = sum;
            medoid = this->node_to_point_map[i];
        }
    }

    return medoid;
    
}
template <typename datatype>
void ANN<datatype>::Vamana(float alpha,int L,int R){
   
    this->G->enforceRegular(R);

    //Get medoid of dataset
    std::vector<datatype> medoid = this->getMedoid();

    //Get a random permutation of 1 to n
    std::vector<int> perm;

    for(size_t i=0;i<this->node_to_point_map.size();i++){
        perm.push_back(i);
    }

    unsigned seed = 0;

    std::shuffle(perm.begin(),perm.end(),std::default_random_engine(seed));

    for(size_t i=0;i<this->node_to_point_map.size();i++){
        int p = perm[i];
        
        std::set<std::vector<datatype>> L_set;
        L_set = this->greedySearch(medoid,this->node_to_point_map[p],1,L);
        
        // Asume greedySearch also returns Visited
        // #TODO This is a placeholder
        std::vector<datatype> point = this->node_to_point_map[p];
        CompareVectors<datatype> compare(point);
        std::set<std::vector<datatype>, CompareVectors<datatype>> Visited(compare);        
        
        this->robustPrune(point,Visited,alpha,R);


        std::vector<std::vector<datatype>> neighbours;
        this->neighbourNodes(this->node_to_point_map[p],neighbours);
        for(auto neighbour:neighbours){
            if((this->G->countNeighbours(this->point_to_node_map[neighbour])+1) > R){
                std::vector<datatype> point = this->node_to_point_map[p];
                CompareVectors<datatype> compare(point);
                std::set<std::vector<datatype>, CompareVectors<datatype>> temp(compare);
                temp.insert(point);

                this->robustPrune(neighbour,temp,alpha,R);
            }
            else{
                this->G->addEdge(this->point_to_node_map[neighbour],p);
            }
        }

    }
}

// Explicit instantiation of ANN class for datatype int, float and unsigned char
template class ANN<int>;
template class ANN<float>;
template class ANN<long>;
template class ANN<unsigned char>;

// Explicit instantiation for robustPrune with the different datatypes
template void ANN<int>::robustPrune<CompareVectors<int>>(
    std::vector<int>, 
    std::set<std::vector<int>, CompareVectors<int>>&, 
    float, 
    int
);
template void ANN<float>::robustPrune<CompareVectors<float>>(
    std::vector<float>, 
    std::set<std::vector<float>, CompareVectors<float>>&, 
    float, 
    int
);
template void ANN<unsigned char>::robustPrune<CompareVectors<unsigned char>>(
    std::vector<unsigned char>, 
    std::set<std::vector<unsigned char>, CompareVectors<unsigned char>>&, 
    float, 
    int
);
template void ANN<long>::robustPrune<CompareVectors<long>>(
    std::vector<long>, 
    std::set<std::vector<long>, CompareVectors<long>>&, 
    float, 
    int
);






/*
Graph Vamana(void){
    Graph * R_Regular_Graph = new graph(...);
    ANN * ann = new ANN(R_Regular_Graph);

    copy ann->G;
    delete ann;
    return copy;

}



*/