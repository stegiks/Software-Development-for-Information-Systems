#include "graph.h"

Graph::Graph(std::size_t n){
    // Randomly generate a graph with n nodes
    srand(time(NULL));
    this->adj_list = std::vector<std::unordered_set<int>>(n);
    for(std::size_t i = 0; i < n; i++){
        for(std::size_t j = 0; j < n; j++){
            if(rand() % 2 == 1){
                this->addEdge(i, j);
            }
        }
    }
}

Graph::Graph(std::vector<std::unordered_set<int>> edges){
    this->adj_list = edges;
}

void Graph::addEdge(int a, int b){
    this->adj_list[a].insert(b);
}

void Graph::removeEdge(int a, int b){
    this->adj_list[a].erase(b);
}

std::vector<int> Graph::getNeighbours(int node){
    std::vector<int> neighbours;
    for(auto neighbour : this->adj_list[node]){
        neighbours.push_back(neighbour);
    }
    return neighbours;
    // ! OR RETURN std::unordered_set with the function signature
    // return this->adj_list[node];
}

void Graph::removeNeighbours(int node){
    this->adj_list[node].clear();
}

int Graph::countNeighbours(int node){
    return this->adj_list[node].size();
}

bool Graph::checkSimilarity(std::vector<std::unordered_set<int>> edges){
    // Check if the adjacency list has the same dimensions as the input matrix
    if(this->adj_list.size() != edges.size())
        return false;

    for(std::size_t i = 0; i < this->adj_list.size(); i++)
        if(this->adj_list[i] != edges[i])
            return false;

    return true;
}

void Graph::printGraph(){
    for(std::size_t i = 0; i < this->adj_list.size(); i++){
        std::cout << "Node " << i << " : ";
        for(auto neighbour : this->adj_list[i]){
            std::cout << neighbour << " ";
        }
        std::cout << std::endl;
    }
}