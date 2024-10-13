#include "utils_ann.h"

Graph::Graph(int n){
    // Randomly generate a graph with n nodes
    srand(time(NULL));
    adj_matrix = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            if(rand() % 2 == 1){
                addEdge(i, j);
            }
        }
    }
}

void Graph::addEdge(int a, int b){
    adj_matrix[a][b] = 1;
}

void Graph::removeEdge(int a, int b){
    adj_matrix[a][b] = 0;
}

std::vector<int> Graph::getNeighbours(int node){
    std::vector<int> neighbours;
    for(int i = 0; i < adj_matrix[node].size(); i++){
        if(adj_matrix[node][i] == 1){
            neighbours.push_back(i);
        }
    }
    return neighbours;
}

void Graph::removeNeighbours(int node){
    for(int i = 0; i < adj_matrix[node].size(); i++){
        adj_matrix[node][i] = 0;
    }
}

int Graph::countNeighbours(int node){
    int count = 0;
    for(int i = 0; i < adj_matrix[node].size(); i++){
        if(adj_matrix[node][i] == 1){
            count++;
        }
    }
    return count;
}