#include <gtest/gtest.h>
#include "ann.h"
TEST(ANNTest, TestGetMedoid){
    std::vector<std::vector<int>> points = {{1, 1, 1}, {2, 2, 5}, {2, 4, 5}, {7, 8, 9}};
    ANN<int> ann(points);
    int expected_medoid = 2;
    EXPECT_EQ(ann.getMedoid(), expected_medoid);
}

// Degree Bound Check
TEST(VamanaIndexingTest, DegreeBound1) {
    std::vector<std::vector<int>> points = {{1, 1, 1}, {2, 2, 2}, {3, 3, 3}, {4, 4, 4}};

    // Make graph fully connected
    std::vector<std::unordered_set<int>> edges = {
        {1, 2},
        {2, 0},
        {3, 0},
        {1, 0},
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
    float alpha = 1.1;   
    int L = 2;           
    int R = 2;           

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

TEST(FilteredVamana, BasicFunct){

    std::vector<std::vector<float>> points = {
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

    std::vector<float> filters = {1.2, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    ANN<float>* ann = new ANN<float>(points, edges, filters);

    int L = 3;
    int R = 2;
    float alpha = 1.1;
    
    ann->filteredVamana(alpha, L, R, 0);

    for (size_t i = 0; i < points.size(); ++i) {
        EXPECT_LE(ann->countNeighbours(i), R) << "Degree bound exceeded for node " << i;

        EXPECT_TRUE(ann->checkFilters());
    }

    delete ann;
}


TEST(StitchedVamana, BasicFunct){
    std::vector<std::vector<float>> points = {
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

    std::vector<float> filters = {1.2, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    ANN<float>* ann = new ANN<float>(points, edges, filters);

    int L_small = 4;
    int R_small = 2;
    int R_stitched = 4;
    float alpha = 1.1;
    
    ann->stitchedVamana(alpha, L_small, R_small, R_stitched);

    for (size_t i = 0; i < points.size(); ++i) {
        EXPECT_LE(ann->countNeighbours(i), R_stitched) << "Degree bound exceeded for node " << i;

        EXPECT_TRUE(ann->checkFilters());
    }

    delete ann;


}