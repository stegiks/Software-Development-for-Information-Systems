#include "ann.h"
#include "defs.h"
#include <filesystem>
namespace fs = std::filesystem;

// Prune the set to retain only the k closest points
template <typename datatype>
template <typename Compare>
void ANN<datatype>::pruneSet(std::set<int, Compare>& myset, std::set<int, Compare>& diff, int k){

    if(myset.size() <= static_cast<std::size_t>(k))
        return;

    // Erase all elements after the kth element
    auto start = myset.begin();
    std::advance(start, k);
    auto bound_value = *start;
    myset.erase(start, myset.end());

    auto bound_iter = diff.find(bound_value);
    if(bound_iter != diff.end()){
        diff.erase(bound_iter, diff.end());
    }
}

// Return the neighbors of a point
template <typename datatype>
void ANN<datatype>::neighbourNodes(const int& point, std::vector<int>& neighbours){

    // Retrieve the node index for the point
    std::unordered_set<int>& neighbour_indices = this->G->getNeighbours(point);

    // Convert the neighbor indices back to points
    for(int index : neighbour_indices)
        neighbours.push_back(index);

}

template <typename datatype>
bool ANN<datatype>::checkFilters(){
    for(size_t i = 0; i < this->node_to_point_map.size(); i++){
        // Get the neighbors of the point
        std::vector<int> neighbours;
        this->neighbourNodes(i, neighbours);
        for(int neighbour : neighbours){
            if(this->node_to_filter_map[i] != this->node_to_filter_map[neighbour]){
                return false;
            }
        }
    }

    return true;
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
ANN<datatype>::ANN(const std::vector<std::vector<datatype>>& points, size_t reg){
    int node_index = 0;

    this->G = new Graph(points.size(), reg);  // Call the Graph constructor with number of points
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
ANN<datatype>::ANN(const std::vector<std::vector<datatype>>& points, const std::vector<float>& filters){
    if(points.size() != filters.size()){
        throw std::invalid_argument("ANN: Number of points and filters do not match");
    }

    // Init an empty graph with number of points
    this->G = new Graph(points.size(), true);

    for(std::size_t i = 0; i < points.size(); i++){
        this->node_to_point_map.push_back(points[i]);
        this->node_to_filter_map.push_back(filters[i]);
        this->point_to_node_map[points[i]] = (int)i;
        this->filter_to_node_map[filters[i]].push_back((int)i);
    }
}

template <typename datatype>
void ANN<datatype>:: printGraph(){
    this->G->printGraph();
}

template <typename datatype>
ANN<datatype>::ANN(const std::vector<std::vector<datatype>>& points, const std::vector<std::unordered_set<int>>& edges, const std::vector<float>& filters){
    if(points.size() != filters.size()){
        throw std::invalid_argument("ANN: Number of points and filters do not match");
    }

    // Init an empty graph with number of points if no edges are provided
    if(edges.empty() || edges.size() != points.size()){
        this->G = new Graph(points.size(), true);
    }
    else{
        this->G = new Graph(edges);
    }

    for(std::size_t i = 0; i < points.size(); i++){
        this->node_to_point_map.push_back(points[i]);
        this->node_to_filter_map.push_back(filters[i]);
        this->point_to_node_map[points[i]] = (int)i;
        this->filter_to_node_map[filters[i]].push_back((int)i);
    }
}

template <typename datatype>
ANN<datatype>::~ANN(){
    if(this->G != nullptr)
        delete this->G;
}


template<typename datatype>
bool ANN<datatype>::checkErrorsGreedy(const int& start, int k, int upper_limit){
    if(this->node_to_point_map.empty()){
        std::cerr   << "Error : Graph is empty" << RESET << std::endl;
        throw std::invalid_argument("greedySearch: Graph is empty");
        return true;
    }

    if(start < -1 || start >= (int)this->node_to_point_map.size()){
        std::cerr   << "Error : Start point is empty" << RESET << std::endl;
        throw std::invalid_argument("greedySearch: Start point is empty");
        return true;
    }


    if(upper_limit < k){
        std::cerr   << "Error : Upper limit cannot be less than k" << RESET << std::endl;
        throw std::invalid_argument("greedySearch: Upper limit cannot be less than k");
        return true;
    }

    return false;
}

template <typename datatype>
bool ANN<datatype>::checkErrorsRobust(const int & point, const float alpha, const int degree_bound){

    if(this->node_to_point_map.empty()){
        std::cerr   << "Error : Graph is empty" << RESET << std::endl;
        throw std::invalid_argument("robustPrune: Graph is empty");
        return true;
    }

    if(point< 0 || (size_t)point >= this->node_to_point_map.size()){
        std::cerr   << "Error : Point is empty" << RESET << std::endl;
        throw std::invalid_argument("robustPrune: Point is empty");
        return true;
    }


    if(alpha < 1.0){
        std::cerr   << "Error : Alpha cannot be less than 1" << RESET << std::endl;
        throw std::invalid_argument("robustPrune: Alpha cannot be less than 1");
        return true;
    }

    if(degree_bound < 0){
        std::cerr   << "Error : Degree bound cannot be negative" << RESET << std::endl;
        throw std::invalid_argument("robustPrune: Degree bound cannot be negative");
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
bool ANN<datatype>::checkNeighbour(int a, int b){
    return this->G->isNeighbour(a,b);
}

// Fill the filter_to_start_node map for testing
template <typename datatype>
void ANN<datatype>::fillFilterToStartNode(std::unordered_map<float, int>& filter_to_start_node){
    for(const auto& pair : filter_to_start_node){
        this->filter_to_start_node[pair.first] = pair.second;
    }
}

// Filtered Greedy Search algorithm to find the nearest neighbours with a filter value
template <typename datatype>
template <typename Compare>
void ANN<datatype>::filteredGreedySearch(const int& start_node, int k, int upper_limit, const float& filter_query_value, std::set<int, Compare>& NNS, std::unordered_set<int>& Visited, CompareVectors<datatype>& compare){
    // Error handling
    if(this->checkErrorsGreedy(start_node, k, upper_limit)){
        NNS.clear();
        return;
    }

    // If the start_node has value -1, then this means we are checking for a unfiltered query.
    // In this case, we are performing a "quick" filtered greedy search to find the nearest node in every sub graph.
    std::set<int, Compare> difference(compare);
    if(start_node == -1){
        std::set<int, Compare> temp_nns(compare);
        std::unordered_set<int> temp_visited;
        int temp_upper_limit = upper_limit < 2 ? upper_limit : 2;
        
        for(const auto& pair : this->filter_to_start_node){
            temp_nns.clear();
            temp_visited.clear();
            temp_nns.insert(pair.second);
            this->filteredGreedySearch(pair.second, 1, temp_upper_limit, pair.first, temp_nns, temp_visited, compare);

            // Insert the node that greedy found to NNS and difference
            NNS.insert(*(temp_nns.begin()));
            difference.insert(*(temp_nns.begin()));
        }
    }
    else{
        // If the start_node is not -1, then NNS contains the start_node already
        difference.insert(start_node);
    } 

    std::vector<int> neighbours;

    while(!difference.empty()){
        // Get the index of the closest point
        int closest_point = *(difference.begin());
        difference.erase(closest_point);

        // Add the closest point to the Visited set
        Visited.insert(closest_point);

        // Get the neighbors of the closest point
        neighbours.clear();
        this->neighbourNodes(closest_point, neighbours);

        for(const int& neighbour : neighbours){
            // Skip if the neighbour has already been visited
            if(Visited.find(neighbour) != Visited.end())
                continue;
            
            // Filtered query handle
            if(start_node != -1 && this->node_to_filter_map[neighbour] != filter_query_value)
                continue;
            
            NNS.insert(neighbour);
            difference.insert(neighbour);
        }

        // Prune NNS to retain upper_limit closest points
        if(NNS.size() > (std::size_t)upper_limit){
            this->pruneSet(NNS, difference, upper_limit);
        }
    }

    if(NNS.size() > (std::size_t)k){
        this->pruneSet(NNS, difference, k);
    }
}

// Greedy search algorithm to find the nearest neighbours
template <typename datatype>
template <typename Compare>
void ANN<datatype>::greedySearch(const int& start, int k, int upper_limit, std::set<int, Compare>& NNS, std::unordered_set<int>& Visited, CompareVectors<datatype>& compare){
    // Error handling
    if(this->checkErrorsGreedy(start, k, upper_limit)){
        NNS.clear();
        return;
    }

    // difference set the first time will have the start node
    std::set<int, CompareVectors<datatype>> difference(compare);
    difference.insert(start);

    // Neighbour vector to use inside the loop
    std::vector<int> neighbours;

    while(!difference.empty()){

        // Get the index of the closest point
        int closest_point = *(difference.begin());
 
        // Get the neighbors of the closest point
        this->neighbourNodes(closest_point, neighbours);

        // Update NNS set with neighbours of closest_point
        for(const auto& neighbour : neighbours){
            NNS.insert(neighbour);
            
            if(Visited.find(neighbour) == Visited.end())
                difference.insert(neighbour);
        }

        neighbours.clear();

        // Update Visited set with closest_point
        Visited.insert(closest_point);
        difference.erase(closest_point);

        // Update NNS to retain upper_limit closest points and update difference set
        if(NNS.size() > static_cast<std::size_t>(upper_limit)){
            this->pruneSet(NNS, difference, upper_limit);
        }
    }

    // Return k closest points to Xq, using regular set
    this->pruneSet(NNS, difference, k);
}

template <typename datatype>
template <typename Compare>
void ANN<datatype>::robustPrune(const int &point, std::set<int, Compare>& candidate_set, const float alpha, const int degree_bound, bool filtered){
     // Error handling
    if(this->checkErrorsRobust(point, alpha, degree_bound))
        return;
    
    std::vector<int> neighbours;
    this->neighbourNodes(point, neighbours);

    for(const auto& neighbour : neighbours){
        candidate_set.insert(neighbour);
    }

    // Remove point p from candidate_set and prune all its neighbours
    candidate_set.erase(point);
    this->G->removeNeighbours(point);

    while(true){

        if(candidate_set.size() == 0)
            break;

        int closest_point = *(candidate_set.begin());

        // Closest point would have been removed from candidate set in alpha comparison step anyway
        candidate_set.erase(closest_point);
        this->G->addEdge(point, closest_point);
        if(this->G->countNeighbours(point) == degree_bound)
            break;

        for(auto it = candidate_set.begin(); it != candidate_set.end();){
            const auto& element = *it;
           
            if(filtered == FILTERED){
                if(!((this->node_to_filter_map[element] == this->node_to_filter_map[closest_point] &&
                    this->node_to_filter_map[element] == this->node_to_filter_map[point]) || this->node_to_filter_map[element] != this->node_to_filter_map[point])){
                    it++;
                    continue;
                }
            }
            auto x = this->node_to_point_map[closest_point];
            auto y = this->node_to_point_map[element];
            auto z = this->node_to_point_map[point];
            std::size_t dim = y.size();

            if(alpha * float(calculateDistance(x, y, dim)) <= float(calculateDistance(y, z, dim))){
                it = candidate_set.erase(it);
            }
            else{
                it++;
            }
        }
    }
}

template <typename datatype>
bool ANN<datatype>::checkFilteredFindMedoid(std::size_t num_of_filters){
    if(this->filter_to_start_node.size() != num_of_filters){
        throw std::invalid_argument("filteredFindMedoid: Filter map size does not match the number of filters");
        return false;
    }

    return true;
}

template <typename datatype>
int ANN<datatype>::getStartNode(float filter){
    if(this->filter_to_start_node.empty()){
        this->filteredFindMedoid(5);
    }

    auto it = this->filter_to_start_node.find(filter);
    if(it == this->filter_to_start_node.end()){
        return -1;
    }

    return it->second;
}

template <typename datatype>
void ANN<datatype>::filteredFindMedoid(int tau){
    if(this->node_to_filter_map.empty()){
        throw std::invalid_argument("filteredFindMedoid: Filter map is empty");
        return;
    }

    // Iterate through all filter values
    for(const auto& pair : this->filter_to_node_map){
        const float& filter = pair.first;
        const std::vector<int>& Pf = pair.second;

        // Randomly take a sample of max tau nodes from Pf
        // ! CHECK FOR BETTER WAY TO SAMPLE RANDOMLY
        std::vector<int> Rf;
        std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        std::sample(Pf.begin(), Pf.end(), std::back_inserter(Rf), tau, rng);

        // Select the first node as it is randomly sampled
        this->filter_to_start_node[filter] = Rf[0];
    }
}

template <typename datatype>
void ANN<datatype>::calculateMedoid(){
    std::size_t n = this->node_to_point_map.size();
    if(n == 0){
        std::cerr   << "Error : No points in the dataset" << RESET << std::endl;
        throw std::invalid_argument("calculateMedoid: No points in the dataset");
        return;
    }

    // Hold the sum of distances for each point
    std::vector<float> sum_distances(n, 0.0);

    std::size_t dim = 0;
    if(this->node_to_point_map.size() > 0)
        dim = this->node_to_point_map[0].size();
    else
        throw std::invalid_argument("calculateMedoid: No points in the dataset");

    // Calculate one time the distance between each pair of points to save time
    for(std::size_t i = 0; i < n; i++){
        for(std::size_t j = i + 1; j < n; j++){
            float distance = calculateDistance(this->node_to_point_map[i], this->node_to_point_map[j], dim);
            sum_distances[i] += distance;
            sum_distances[j] += distance;
        }
    }

    // Find the point with the minimum sum of distances
    auto min_iterator = std::min_element(sum_distances.begin(), sum_distances.end());
    std::size_t index_min = std::distance(sum_distances.begin(), min_iterator);
    this->cached_medoid = index_min;
}

template <typename datatype>
void ANN<datatype>::randomMedoid(){
    std::size_t n = this->node_to_point_map.size();
    if(n == 0){
        std::cerr   << "Error : No points in the dataset" << RESET << std::endl;
        throw std::invalid_argument("randomMedoid: No points in the dataset");
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> dis(0, n-1);

    this->cached_medoid = dis(gen);
}

template <typename datatype>
const int& ANN<datatype>::getMedoid(){
    if(!this->cached_medoid.has_value())
        this->calculateMedoid();

    return this->cached_medoid.value();
}

template <typename datatype>
void ANN<datatype>::Vamana(float alpha, int L, int R){
    
    this->G->enforceRegular(R);

    // Calculate medoid of dataset
    #if defined(OPTIMIZED)
        this->randomMedoid();
    #else
        this->calculateMedoid();
    #endif

    // Get a random permutation of 1 to n
    std::vector<int> perm;

    for(size_t i=0;i<this->node_to_point_map.size();i++){
        perm.push_back(i);
    }

    unsigned seed = 0;
    std::shuffle(perm.begin(), perm.end(), std::default_random_engine(seed));

    // Neighbours vectors to use inside the loop
    std::vector<int> neighbours;
    std::vector<int> neighbours_j;

    for(size_t i = 0; i < this->node_to_point_map.size(); i++){
        int point = perm[i];

        // Get the point corresponding to the node
        // Create the NNS and Visited sets and pass them as references

        CompareVectors<datatype> compare(this->node_to_point_map, this->node_to_point_map[point]);
        std::set<int, CompareVectors<datatype>> NNS(compare);
        std::unordered_set<int> Visited;
        NNS.insert(this->cached_medoid.value());
        
        // Return k closest points to Xq (point) and then with robust find "better" neighbours
        this->greedySearch(this->cached_medoid.value(), 1, L, NNS, Visited, compare);

        // Transform Visited to a set with a custom comparator
        std::set<int, CompareVectors<datatype>> VisitedRobust(compare);
        for(auto it = Visited.begin(); it != Visited.end(); it++){
            VisitedRobust.insert(*it);
        }

        this->robustPrune(point, VisitedRobust, alpha, R, UNFILTERED);

        this->neighbourNodes(point, neighbours);

        for(auto j : neighbours){
            
            // If j hasn't an outgoing edge to point, then offset is 1
            int offset = this->checkNeighbour(j,point) ? 0 : 1;
            // int offset = 0;
            if((this->G->countNeighbours(j) + offset) > R){
                std::set<int, CompareVectors<datatype>> temp(compare);
                
                this->neighbourNodes(j, neighbours_j);
                neighbours_j.push_back(point);

                for(auto k : neighbours_j){
                    temp.insert(k);
                }

                neighbours_j.clear();
                
                // Call robust for j neighbours
                this->robustPrune(j, temp, alpha, R, UNFILTERED);
            }
            else{
                // Make an edge between j and point too
                this->G->addEdge(j, point);
            }
        }

        neighbours.clear();
    }
}

template <typename datatype>
void ANN<datatype>::filteredPruning(){
    // Iterate over all the edges and if the filter values are different, remove the edge
    for(size_t i = 0; i < this->G->getNumberOfNodes(); i++){
        std::unordered_set<int>& neighbours = this->G->getNeighbours(i);
        std::vector<int> to_remove;

        for(int neighbour : neighbours){
            if(this->node_to_filter_map[i] != this->node_to_filter_map[neighbour]){
                to_remove.push_back(neighbour);
            }
        }

        for(int neighbour : to_remove){
            this->G->removeEdge(i, neighbour);
        }
    }
}

template <typename datatype>
void ANN<datatype>::stitchedVamana(float alpha, int L_small, int R_small, int R_stitched, int z){
    
    this->G->enforceRegular(z);

    ANN<datatype> *small_graph;

    // Iterate over all the filters and create a subgraph for each filter
    for(const auto& pair : this->filter_to_node_map){       
        // Create a vector of points for the filter
        std::vector<std::vector<datatype>> small_points;
        for(int node : pair.second){
            small_points.push_back(this->node_to_point_map[node]);
        }

        // Create a new graph for the filter
        small_graph = new ANN<datatype>(small_points);
        small_graph->Vamana(alpha, L_small, R_small);
        
        // Stitch the small graph to the main graph 
        for(size_t i = 0; i < small_points.size(); i++){
            int node = pair.second[i];
            std::vector<int> neighbours;
            small_graph->neighbourNodes(int(i), neighbours);

            for(int neighbour : neighbours){
                this->G->addEdge(node, pair.second[neighbour]);
            }
        }

        delete small_graph;

    }

    // For each vertice in the graph run robust prune
    for(size_t i = 0; i < this->node_to_point_map.size(); i++){
        std::set<int> candidate_set;
        std::vector<int> neighbours;
        this->neighbourNodes(i, neighbours);

        for(int neighbour : neighbours){
            candidate_set.insert(neighbour);
        }

        this->robustPrune(i, candidate_set, alpha, R_stitched, FILTERED);
    }

}

template <typename datatype>
void ANN<datatype>::filteredVamana(float alpha, int L, int R, int tau, int z){
    
    this->G->enforceRegular(z);

    // Calculate medoid of dataset
    this->filteredFindMedoid(tau);

    // Get a random permutation of 1 to n
    std::vector<int> perm;

    for(size_t i=0;i<this->node_to_point_map.size();i++){
        perm.push_back(i);
    }

    unsigned seed = 0;
    std::shuffle(perm.begin(), perm.end(), std::default_random_engine(seed));

    // Neighbours vectors to use inside the loop
    std::vector<int> neighbours;
    std::vector<int> neighbours_j;

    for(size_t i = 0; i < this->node_to_point_map.size(); i++){
        int point = perm[i];

        CompareVectors<datatype> compare(this->node_to_point_map, this->node_to_point_map[point]);
        std::set<int, CompareVectors<datatype>> NNS(compare);
        std::unordered_set<int> Visited;

        int temporary_point = this->filter_to_start_node[this->node_to_filter_map[point]];
        float filter = this->node_to_filter_map[point];

        NNS.insert(temporary_point);

        // Return k closest points to Xq (point) and then with robust find "better" neighbours
        this->filteredGreedySearch(temporary_point, 1, L, filter, NNS, Visited, compare);

        // Transform Visited to a set with a custom comparator
        std::set<int, CompareVectors<datatype>> VisitedRobust(compare);
        for(auto it = Visited.begin(); it != Visited.end(); it++){
            VisitedRobust.insert(*it);
        }

        this->robustPrune(point, VisitedRobust, alpha, R, FILTERED);

        this->neighbourNodes(point, neighbours);

        for(auto j : neighbours){
            
            this->G->addEdge(j, point);

            if(this->G->countNeighbours(j) > R){
                // Call robust for j neighbours
                std::set<int, CompareVectors<datatype>> temp(compare);

                this->neighbourNodes(j, neighbours_j);
                for(auto k : neighbours_j){
                    temp.insert(k);
                }

                neighbours_j.clear();
                this->robustPrune(j, temp, alpha, R, FILTERED);
            }
        }
        neighbours.clear();
    }
}

template <typename datatype>
void ANN<datatype>::saveGraph(const std::string& file_path) {
    namespace fs = std::filesystem;
    if (fs::exists(file_path)) {
        std::cerr << "Error: File \"" << file_path << "\" already exists.\n";
        throw std::invalid_argument("saveGraph: File already exists");
    }

    std::ofstream out_file(file_path, std::ios::binary);
    if (!out_file) {
        std::cerr << "Error: Could not create file \"" << file_path << "\".\n";
        throw std::invalid_argument("saveGraph: Could not open file");
    }

    Graph* G = this->G;
    const std::size_t num_nodes = G->getNumberOfNodes();
    out_file.write(reinterpret_cast<const char*>(&num_nodes), sizeof(num_nodes));

    for (std::size_t i = 0; i < num_nodes; ++i) {
        const auto& neighbours = G->getNeighbours(i);
        const std::size_t num_neighbours = neighbours.size();
        out_file.write(reinterpret_cast<const char*>(&i), sizeof(i));
        out_file.write(reinterpret_cast<const char*>(&num_neighbours), sizeof(num_neighbours));

        for (const int neighbour : neighbours) {
            out_file.write(reinterpret_cast<const char*>(&neighbour), sizeof(neighbour));
        }
    }

    out_file.close();
}

template <typename datatype>
void ANN<datatype>::loadGraph(const std::string& file_path) {
    std::ifstream in_file(file_path, std::ios::binary);
    if (!in_file) {
        std::cerr << "Error: Could not open file \"" << file_path << "\".\n";
        throw std::invalid_argument("loadGraph: Could not open file");
    }

    std::size_t num_nodes;
    in_file.read(reinterpret_cast<char*>(&num_nodes), sizeof(num_nodes));

    // Clear the old graph if necessary
    if (this->G != nullptr) {
        delete this->G;
    }
    this->G = new Graph(num_nodes, true);

    for (std::size_t i = 0; i < num_nodes; ++i) {
        std::size_t node, num_neighbours;
        in_file.read(reinterpret_cast<char*>(&node), sizeof(node));
        in_file.read(reinterpret_cast<char*>(&num_neighbours), sizeof(num_neighbours));

        for (std::size_t j = 0; j < num_neighbours; ++j) {
            int neighbour;
            in_file.read(reinterpret_cast<char*>(&neighbour), sizeof(neighbour));
            this->G->addEdge(node, neighbour);
        }
    }

    in_file.close();
}

// Explicit instantiation of ANN class for datatype int, float and unsigned char
template class ANN<int>;
template class ANN<float>;
template class ANN<unsigned char>;

// Explicit instantiation for filteredGreedySearch with the different datatypes
template void ANN<int>::filteredGreedySearch<CompareVectors<int>>(
    const int&, 
    int, 
    int, 
    const float&, 
    std::set<int, CompareVectors<int>>&, 
    std::unordered_set<int>&, 
    CompareVectors<int>&
);
template void ANN<float>::filteredGreedySearch<CompareVectors<float>>(
    const int&, 
    int, 
    int, 
    const float&, 
    std::set<int, CompareVectors<float>>&, 
    std::unordered_set<int>&, 
    CompareVectors<float>&
);
template void ANN<unsigned char>::filteredGreedySearch<CompareVectors<unsigned char>>(
    const int&, 
    int, 
    int, 
    const float&, 
    std::set<int, CompareVectors<unsigned char>>&, 
    std::unordered_set<int>&, 
    CompareVectors<unsigned char>&
);

// Explicit instantiation for greedySearch with the different datatypes
template void ANN<int>::greedySearch<CompareVectors<int>>(
    const int&, 
    int, 
    int, 
    std::set<int, CompareVectors<int>>&, 
    std::unordered_set<int>&, 
    CompareVectors<int>&
);
template void ANN<float>::greedySearch<CompareVectors<float>>(
    const int&, 
    int, 
    int, 
    std::set<int, CompareVectors<float>>&, 
    std::unordered_set<int>&, 
    CompareVectors<float>&
);
template void ANN<unsigned char>::greedySearch<CompareVectors<unsigned char>>(
    const int&, 
    int, 
    int, 
    std::set<int, CompareVectors<unsigned char>>&, 
    std::unordered_set<int>&, 
    CompareVectors<unsigned char>&
);

// Explicit instantiation for robustPrune with the different datatypes
template void ANN<int>::robustPrune<CompareVectors<int>>(
    const int&, 
    std::set<int, CompareVectors<int>>&, 
    const float, 
    const int,
    bool
);

template void ANN<float>::robustPrune<CompareVectors<float>>(
    const int&, 
    std::set<int, CompareVectors<float>>&, 
    const float, 
    const int,
    bool
);

template void ANN<unsigned char>::robustPrune<CompareVectors<unsigned char>>(
    const int&, 
    std::set<int, CompareVectors<unsigned char>>&, 
    const float, 
    const int,
    bool
);
