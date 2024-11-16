#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <ctime>
#include <algorithm>
#include <unordered_set>
#include <stdexcept>
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator> 

// Class to represent a graph
class Graph{
private:
    // Adjacency matrix to store graph edges
    std::vector<std::unordered_set<int>> adj_list;
    std::size_t num_nodes;

public:
    Graph(std::size_t n, bool init_empty = false);
    Graph(std::size_t n, size_t reg);
    Graph(std::vector<std::unordered_set<int>> edges);

    // Function to add and remove edges
    void addEdge(int a, int b);
    void removeEdge(int a, int b);
    
    // Function to get neighbours of a node
    std::unordered_set<int>& getNeighbours(int node);

    // Function to determine if a node is a neighbour
    bool isNeighbour(int node, int neighbour);

    void removeNeighbours(int node);
    int countNeighbours(int node);

    bool checkSimilarity(std::vector<std::unordered_set<int>> edges);
    void printGraph();

    void enforceRegular(int R);
};

#endif // graph.h