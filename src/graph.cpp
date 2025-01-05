#include "graph.h"
#include <random>

Graph::Graph(std::size_t n, bool init_empty){
    // Randomly generate a graph with n nodes
    this->num_nodes = n;
    this->adj_list = std::vector<std::unordered_set<int>>(n);

    if(init_empty)
        return;

    // If not empty, randomly generate edges
    srand(time(NULL));
    for(std::size_t i = 0; i < n; i++){
        for(std::size_t j = 0; j < n; j++){
            if(rand() % 2 == 1 && i != j){
                this->adj_list.at(i).insert(j);
            }
        }
    }    
}


Graph::Graph(std::size_t n,size_t reg){
    // Randomly generate a graph with n nodes
    srand(time(NULL));
    this->adj_list = std::vector<std::unordered_set<int>>(n);
    this->num_nodes = n;

    std::vector<int> permutation;

    for(size_t i=0;i<n;i++){
        permutation.push_back(i);
    }

    for(std::size_t i = 0; i < n; i++){
        std::shuffle(permutation.begin(), permutation.end(), std::default_random_engine(i));
        for(std::size_t j = 0; j < reg; j++){
            if(i != j){
                this->adj_list.at(i).insert(permutation.at(j));
            }
        }
    }
}

Graph::Graph(std::vector<std::unordered_set<int>> edges){
    this->adj_list = edges;
    this->num_nodes = edges.size();
}

void Graph::addEdge(int a, int b){
    if((std::size_t)a >= this->num_nodes || (std::size_t)b >= this->num_nodes){
        throw std::out_of_range("Node index out of range");
    }
    this->adj_list.at(a).insert(b);
}

void Graph::removeEdge(int a, int b){
    if((std::size_t)a >= this->num_nodes || (std::size_t)b >= this->num_nodes){
        throw std::out_of_range("Node index out of range");
    }
    this->adj_list.at(a).erase(b);
}

std::unordered_set<int>& Graph::getNeighbours(int node){
    if((std::size_t)node >= this->num_nodes){
        throw std::out_of_range("Node index out of");
    }
    return this->adj_list.at(node);
}

void Graph::removeNeighbours(int node){
    if((std::size_t)node >= this->num_nodes){
        throw std::out_of_range("Node index out of range");
    }
    this->adj_list.at(node).clear();
}

int Graph::countNeighbours(int node){
    if((std::size_t)node >= this->num_nodes){
        throw std::out_of_range("Node index out of range");
    }
    return this->adj_list.at(node).size();
}

bool Graph::isNeighbour(int node, int neighbour){
    if((std::size_t)node >= this->num_nodes){
        throw std::out_of_range("Node index out of range");
    }
    return this->adj_list.at(node).find(neighbour) != this->adj_list.at(node).end();
}

bool Graph::checkSimilarity(std::vector<std::unordered_set<int>> edges){
    // Check if the adjacency list has the same dimensions as the input matrix
    if(this->adj_list.size() != edges.size())
        return false;

    for(std::size_t i = 0; i < this->adj_list.size(); i++)
        if(this->adj_list.at(i) != edges.at(i))
            return false;

    return true;
}

void Graph::printGraph(){
    for(std::size_t i = 0; i < this->adj_list.size(); i++){
        std::cout << "Node " << i << " : ";
        for(auto neighbour : this->adj_list.at(i)){
            std::cout << neighbour << " ";
        }
        std::cout << std::endl;
    }
}

// If the graph is not regular, enforce it to be regular
void Graph::enforceRegular(int R){
    // Possible Parallelization Section
    for(std::size_t i = 0; i < this->adj_list.size(); i++){
        std::unordered_set<int>& neighbours = this->getNeighbours(i);

        // Set upper limit to be the minimum of R and number of nodes

        // If node has more than R neighbors, remove random edges
        if(neighbours.size() > static_cast<size_t>(R)){
            std::vector<int> neighboursVec(neighbours.begin(), neighbours.end());
            std::shuffle(neighboursVec.begin(),neighboursVec.end(),std::default_random_engine(0));
            for(std::size_t j = R; j < neighboursVec.size(); ++j){
                neighbours.erase(neighboursVec[j]);
            }
        }

        size_t upper_limit = this->adj_list.size() < static_cast<size_t>(R) ? this->adj_list.size()-1 : static_cast<size_t>(R);
        
        // If node has fewer than R neighbors, add random edges
        while(neighbours.size() < upper_limit){
            std::size_t j = rand() % this->adj_list.size();
            if(i != j && neighbours.find(j) == neighbours.end()){
                neighbours.insert(j);
            }
        }
    }
}


std::size_t Graph::getNumberOfNodes(){
    return this->num_nodes;
}