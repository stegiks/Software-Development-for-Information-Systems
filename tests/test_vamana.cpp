#include <gtest/gtest.h>
#include "ann.h"

// Degree Bound Check
TEST(VamanaIndexingTest, DegreeBound1) {
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}};

    // Make graph fully connected
    std::vector<std::unordered_set<int>> edges = {
        {1, 2, 3, 4},
        {0, 2, 3, 4},
        {0, 1, 3, 4},
        {0, 1, 2, 4},
        {0, 1, 2, 3}
    };
    int L = 3;
    int R = 2;
    float alpha = 1.1;

    ANN<int> ann(points,edges);
    ann.Vamana(alpha, L, R);

    for (size_t i = 0; i < points.size(); ++i) {
        printf("Node %ld has degree %d\n", i, ann.countNeighbours(i));
        EXPECT_LE(ann.countNeighbours(i), R) << "Degree bound exceeded for node " << i;
    }
}



TEST(VamanaIndexingTest, DegreeBound2) {
    using datatype = float;

    // Sample points in a 2D space
    std::vector<std::vector<datatype>> points = {
        {0.0, 0.0}, {1.0, 1.0}, {2.0, 2.0}, {3.0, 3.0}, {4.0, 4.0}, {5.0, 5.6}, {10.0, 11.0}
    };

    // Predefined edges to control connectivity explicitly (assuming undirected edges)
    std::vector<std::unordered_set<int>> edges = {
        {1, 2, 3, 4, 5},
        {0, 2, 3, 4, 5},
        {0, 1, 3, 4, 5},
        {0, 1, 2, 4, 5},
        {0, 1, 2, 3, 5},
        {0, 1, 2, 3, 4, 6},
        {5}
    };
    // Initialize ANN with points and predefined edges
    ANN<datatype> ann(points, edges);

    // Set Vamana algorithm parameters
    float alpha = 1.1;   // Controls the pruning aggressiveness
    int L = 2;           // Number of potential connections to explore per node
    int R = 2;           // Max neighbors each node can retain after pruning

    // Run the Vamana algorithm
    ann.Vamana(alpha, L, R);

    // Retrieve the graph structure by checking the neighbor count
    for (size_t i = 0; i < points.size(); ++i) {
        // Count neighbors of the current node
        int neighbor_count = ann.countNeighbours(i);

        // Verify that each node has no more than R neighbors
        EXPECT_LE(neighbor_count, R)
            << "Node " << i << " has more than R neighbors.";
    }
}
