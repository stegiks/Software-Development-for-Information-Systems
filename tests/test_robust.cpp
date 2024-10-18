#include <gtest/gtest.h>
#include "ann.h"
#include <set>
#include <vector>

// Test basic pruning functionality
TEST(RobustPrune, BasicFunctionality) {
    // Initialize a set of points and an ANN graph
    std::vector<std::vector<int>> points = {{1, 2}, {1, 0}, {2, 3}, {1, -5}, {3, -5}, {6, 2}};
    std::vector<std::vector<int>> edges = {
        {0, 0, 1, 0, 0, 0},
        {1, 0, 0, 1, 0, 0},
        {0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 1, 0}
    };

    std::vector<int> start = {2, 3};
    std::set<std::vector<int>> candidate = {{1, 0}, {1, 2}, {1, -5}};
    std::vector<std::vector<int>> expected = {
        {0, 0, 1, 0, 0, 0},
        {1, 0, 0, 1, 0, 0},
        {1, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 1, 0}
    };
    ANN<int> ann(points, edges);

    ann.robustPrune(start, candidate, 1.1, 5);


    EXPECT_TRUE(ann.checkGraph(expected));
}

