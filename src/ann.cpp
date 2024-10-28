#include "ann.h"
#include "defs.h"

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
    std::unordered_set<int>& neighbour_indices = this->G->getNeighbours(node);

    // Convert the neighbor indices back to points
    for(int index : neighbour_indices)
        neighbours.push_back(this->node_to_point_map[index]);

}

template <typename datatype>
int ANN<datatype>::countNeighbours(int node){
    return this->G->countNeighbours(node);
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
ANN<datatype>::ANN(const std::vector<std::vector<datatype>>& points, const std::vector<std::unordered_set<int>>& edges) {
    std::size_t num_nodes = points.size();

    int node_index = 0;

    for(const auto& point : points){
        this->node_to_point_map.push_back(point);
        this->point_to_node_map[point] = node_index;
        node_index++;
    }

    if(edges.empty() || edges.size() != num_nodes){
        this->G = new Graph(num_nodes);  // Initialize graph with number of points
    }
    else{
        this->G = new Graph(edges);  // Initialize graph with edges
    } 
}

template <typename datatype>
ANN<datatype>::~ANN(){
    if(this->G != nullptr)
        delete this->G;
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

template <typename datatype>
bool ANN<datatype>::checkGraph(std::vector<std::unordered_set<int>> edges){
    return this->G->checkSimilarity(edges);
}

// Check if a has an outgoing edge to b
template <typename datatype>
bool ANN<datatype>::checkNeighbour(std::vector<datatype> a, std::vector<datatype> b){
    return this->G->isNeighbour(this->point_to_node_map[a], this->point_to_node_map[b]);
}

// Greedy search algorithm to find the nearest neighbours
template <typename datatype>
template <typename Compare>
void ANN<datatype>::greedySearch(const std::vector<datatype>& start, const std::vector<datatype>& query, int k, int upper_limit, std::set<std::vector<datatype>, Compare>& NNS, std::set<std::vector<datatype>, Compare>& Visited, CompareVectors<datatype>& compare){
    
    // Error handling
    if(this->checkErrorsGreedy(start, query, k, upper_limit)){
        NNS.clear();
        return;
    }

    while(true){
        std::set<std::vector<datatype>, CompareVectors<datatype>> difference(compare);
        std::set_difference(NNS.begin(), NNS.end(), Visited.begin(), Visited.end(),
                    std::inserter(difference, difference.end()), compare);

        // If the difference is empty, break out of the loop
        if(difference.empty())
            break;

        // Get the index of the closest point
        std::vector<datatype> closest_point = *(difference.begin());
 
        // Get the neighbors of the closest point
        std::vector<std::vector<datatype>> neighbours;
        this->neighbourNodes(closest_point, neighbours);

        // Update NNS set with neighbours of closest_point
        for(const auto& neighbour : neighbours)
            NNS.insert(neighbour);

        // Update Visited set with closest_point
        Visited.insert(closest_point);

        // Update NNS to retain upper_limit closest points
        if(NNS.size() > static_cast<std::size_t>(upper_limit))
            this->pruneSet(NNS, upper_limit);
        
    }

    // Return k closest points to Xq, using regular set
    this->pruneSet(NNS, k);
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

            // printf("Comparing %f and %f\nPoints: closest_point %d, element %d, point %d\n", alpha * float(calculateDistance(closest_point, element)), float(calculateDistance(element, point)),this->point_to_node_map[closest_point],this->point_to_node_map[element],this->point_to_node_map[point]);
            if(alpha * float(calculateDistance(closest_point, element)) <= float(calculateDistance(element, point)))
                it = candidate_set.erase(it);
            else
                it++;  // Only advance iterator if no deletion
        }
    }
}

template <typename datatype>
std::vector<datatype> ANN<datatype>::getMedoid(){
    std::vector<datatype> medoid;

    // Get the point with the minimum sum of distances
    float min_sum = std::numeric_limits<float>::max();

    for(size_t i=0; i<this->node_to_point_map.size(); i++){
        
        float sum = 0;
        for(size_t j=0; j<this->node_to_point_map.size(); j++){
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
void ANN<datatype>::Vamana(float alpha, int L, int R){
   
    this->G->enforceRegular(R);

    //Get medoid of dataset
    std::vector<datatype> medoid = this->getMedoid();

    //Get a random permutation of 1 to n
    std::vector<int> perm;

    for(size_t i=0;i<this->node_to_point_map.size();i++){
        perm.push_back(i);
    }

    unsigned seed = 0;
    std::shuffle(perm.begin(), perm.end(), std::default_random_engine(seed));

    for(size_t i = 0; i < this->node_to_point_map.size(); i++){
        int p = perm[i];

        // Get the point corresponding to the node
        // Create the NNS and Visited sets and pass them as references
        std::vector<datatype> point = this->node_to_point_map[p];
        CompareVectors<datatype> compare(point);
        std::set<std::vector<datatype>, CompareVectors<datatype>> NNS(compare);
        std::set<std::vector<datatype>, CompareVectors<datatype>> Visited(compare);
        NNS.insert(medoid);
        
        // Return k closest points to Xq (point) and then with robust find "better" neighbours
        this->greedySearch(medoid, point, 1, L, NNS, Visited, compare);        
        this->robustPrune(point, Visited, alpha, R);

        std::vector<std::vector<datatype>> neighbours;
        this->neighbourNodes(point, neighbours);

        for(auto j : neighbours){
            
            // If j hasn't an outgoing edge to point, then offset is 1
            int offset = this->checkNeighbour(j,point) ? 0 : 1;
            // int offset = 0;
            if((this->G->countNeighbours(this->point_to_node_map[j]) + offset) > R){
                std::set<std::vector<datatype>, CompareVectors<datatype>> temp(compare);
                
                std::vector<std::vector<datatype>> neighbours_j;
                this->neighbourNodes(j, neighbours_j);
                neighbours_j.push_back(point);

                for(auto k : neighbours_j){
                    temp.insert(k);
                }
                
                // Call robust for j neighbours
                this->robustPrune(j, temp, alpha, R);
            }
            else{
                // Make an edge between j and point too
                this->G->addEdge(this->point_to_node_map[j], p);
            }
        }
    }
}

// Explicit instantiation of ANN class for datatype int, float and unsigned char
template class ANN<int>;
template class ANN<float>;
template class ANN<long>;
template class ANN<unsigned char>;

// Explicit instantiation for greedySearch with the different datatypes
template void ANN<int>::greedySearch<CompareVectors<int>>(
    const std::vector<int>&, 
    const std::vector<int>&, 
    int, 
    int, 
    std::set<std::vector<int>, CompareVectors<int>>&, 
    std::set<std::vector<int>, CompareVectors<int>>&, 
    CompareVectors<int>&
);
template void ANN<float>::greedySearch<CompareVectors<float>>(
    const std::vector<float>&, 
    const std::vector<float>&, 
    int, 
    int, 
    std::set<std::vector<float>, CompareVectors<float>>&, 
    std::set<std::vector<float>, CompareVectors<float>>&, 
    CompareVectors<float>&
);
template void ANN<unsigned char>::greedySearch<CompareVectors<unsigned char>>(
    const std::vector<unsigned char>&, 
    const std::vector<unsigned char>&, 
    int, 
    int, 
    std::set<std::vector<unsigned char>, CompareVectors<unsigned char>>&, 
    std::set<std::vector<unsigned char>, CompareVectors<unsigned char>>&, 
    CompareVectors<unsigned char>&
);
template void ANN<long>::greedySearch<CompareVectors<long>>(
    const std::vector<long>&, 
    const std::vector<long>&, 
    int, 
    int, 
    std::set<std::vector<long>, CompareVectors<long>>&, 
    std::set<std::vector<long>, CompareVectors<long>>&, 
    CompareVectors<long>&
);



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
