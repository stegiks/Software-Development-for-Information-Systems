#include "graph.h"
#include <random>
Graph::Graph(std::size_t n){
    // Randomly generate a graph with n nodes
    srand(time(NULL));
    this->adj_list = std::vector<std::unordered_set<int>>(n);
    for(std::size_t i = 0; i < n; i++){
        for(std::size_t j = 0; j < n; j++){
            if(rand() % 2 == 1 && i != j){
                this->addEdge(i, j);
            }
        }
    }
    this->num_nodes = n;
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

const std::unordered_set<int>& Graph::getNeighbours(int node){
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

void Graph::enforceRegular(int R){
    for(std::size_t i = 0; i < this->adj_matrix.size(); i++){
        std::vector<int> neighbours = getNeighbours(i);
        if(neighbours.size() >size_t(R)){
            std::shuffle(neighbours.begin(), neighbours.end(),std::default_random_engine(0));
            for(std::size_t j = R; j < neighbours.size(); j++){
                removeEdge(i, neighbours[j]);
            }
        }
    }
}