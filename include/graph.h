#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <unordered_set>

// Class to represent a graph
class Graph{
private:
    // Adjacency matrix to store graph edges
    std::vector<std::unordered_set<int>> adj_list;

public:
    Graph(std::size_t n);
    Graph(std::vector<std::unordered_set<int>> edges);

    // Function to add and remove edges
    void addEdge(int a, int b);
    void removeEdge(int a, int b);
    
    //Function to get neighbours of a node
    std::vector<int> getNeighbours(int node);

    void removeNeighbours(int node);
    int countNeighbours(int node);

    bool checkSimilarity(std::vector<std::unordered_set<int>> edges);
    void printGraph();
};

#endif // graph.h