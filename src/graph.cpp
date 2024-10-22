#include "graph.h"

Graph::Graph(std::size_t n){
    // Randomly generate a graph with n nodes
    srand(time(NULL));
    this->adj_matrix = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
    for(std::size_t i = 0; i < n; i++){
        for(std::size_t j = 1; j < n; j++){
            if((rand() % 2 == 1) && i != j){
                addEdge(i, j);
            }
        }
    }
}

Graph::Graph(std::size_t n,int p){
    // Randomly generate a graph with n nodes
    srand(time(NULL));
    this->adj_matrix = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
    for(std::size_t i = 0; i < n; i++){
        // Get p random neighbours
        for(int j = 0; j < p; j++){
            int neighbour;
            do{
                neighbour = rand() % n;
            }while(neighbour == i || this->adj_matrix[i][neighbour] == 1);
            addEdge(i, neighbour);
        }
    }
}

Graph::Graph(std::vector<std::vector<int>> edges) : adj_matrix(edges){}

void Graph::addEdge(int a, int b){
    this->adj_matrix[a][b] = 1;
}

void Graph::removeEdge(int a, int b){
    this->adj_matrix[a][b] = 0;
}

std::vector<int> Graph::getNeighbours(int node){
    std::vector<int> neighbours;
    for(std::size_t i = 0; i < this->adj_matrix[node].size(); i++){
        if(this->adj_matrix[node][i] == 1){
            neighbours.push_back(i);
        }
    }
    return neighbours;
}

void Graph::removeNeighbours(int node){
    for(std::size_t i = 0; i < this->adj_matrix[node].size(); i++){
        this->adj_matrix[node][i] = 0;
    }
}

int Graph::countNeighbours(int node){
    int count = 0;
    for(std::size_t i = 0; i < this->adj_matrix[node].size(); i++){
        if(this->adj_matrix[node][i] == 1){
            count++;
        }
    }
    return count;
}

bool Graph::checkSimilarity(std::vector<std::vector<int>> edges){
    // Check if the adjacency matrix has the same dimensions as the input matrix
    if(this->adj_matrix.size() != edges.size())
        return false;


    for(std::size_t i = 0; i < this->adj_matrix.size(); i++)
        for(std::size_t j = 0; j < this->adj_matrix[i].size(); j++)
            if(this->adj_matrix[i][j] != edges[i][j])
                return false;

    return true;
}

void Graph::printGraph(){
    for(std::size_t i = 0; i < this->adj_matrix.size(); i++){
        for(std::size_t j = 0; j < this->adj_matrix[i].size(); j++){
            std::cout << this->adj_matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}