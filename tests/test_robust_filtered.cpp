#include <gtest/gtest.h>
#include "ann.h"
#include <set>
#include <vector>

// Test basic pruning functionality
TEST(FilteredRobustPruneTest, BasicFunctionality){


    std::vector<std::vector<int>> points = {{1, 2}, {1, 0}, {2, 3}, {1, -5}, {3, -5}, {6, 2}};
    std::vector<std::unordered_set<int>> edges = {
        {2},
        {0, 3},
        {1, 5},
        {},
        {3},
        {4}
    };

    std::vector<int> start = {2, 3};
    std::vector<float> filters = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    ANN<int>* ann = new ANN<int>(points, edges, filters);
    
    // Provide custom comparator for the set
    CompareVectors<int> compare(ann->node_to_point_map,start);
    std::set<int, CompareVectors<int>> candidate(compare);
    candidate.insert(0);
    candidate.insert(1);
    candidate.insert(3);

    std::vector<std::unordered_set<int>> expected = {
        {2},
        {0, 3},
        {0, 5},
        {},
        {3},
        {4}
    };

    int start_node = 2;
    ann->robustPrune(start_node, candidate, 1.1, 5, FILTERED);

    EXPECT_TRUE(ann->checkGraph(expected));
}

// Test basic pruning functionality
TEST(FilteredRobustPruneTest, FilteredFunctionality1){


    std::vector<std::vector<int>> points = {{1, 2}, {1, 0}, {2, 3}, {1, -5}, {3, -5}, {6, 2}};
    std::vector<std::unordered_set<int>> edges = {
        {},
        {3},
        {1, 5},
        {},
        {3},
        {4}
    };

    std::vector<int> start = {2, 3};
    std::vector<float> filters = {1.2, 1.0, 1.0, 1.0, 1.0, 1.0};
    ANN<int>* ann = new ANN<int>(points, edges, filters);
    
    // Provide custom comparator for the set
    CompareVectors<int> compare(ann->node_to_point_map,start);
    std::set<int, CompareVectors<int>> candidate(compare);
    candidate.insert(1);
    candidate.insert(3);

    std::vector<std::unordered_set<int>> expected = {
        {},
        {3},
        {1, 5},
        {},
        {3},
        {4}
    };

    int start_node = 2;
    ann->robustPrune(start_node, candidate, 1.1, 5, FILTERED);

    EXPECT_TRUE(ann->checkGraph(expected));
}



TEST(FilteredRobustPruneTest, FilteredFunctionality2){

    std::vector<std::vector<int>> points = {
        {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}
    };
    std::vector<std::unordered_set<int>> edges = {
        {1, 2}, {0, 3}, {0}, {}, {3, 5}, {4}
    };

    std::vector<float> filters = {1.3, 1.3, 1.3, 1.0, 2.0, 2.0};
    ANN<int>* ann = new ANN<int>(points, edges, filters);

    // Provide custom comparator for the set
    std::vector<int> start = {0, 1};
    CompareVectors<int> compare(ann->node_to_point_map, start);
    std::set<int, CompareVectors<int>> candidate(compare);
    candidate.insert(2);

    // Expected graph after pruning
    std::vector<std::unordered_set<int>> expected = {
        {1}, {0, 3}, {0}, {}, {3, 5}, {4}
    };

    // Perform pruning with FILTERED enabled
    int start_node = 0;
    ann->robustPrune(start_node, candidate, 1.1, 2, FILTERED);


    ann->printGraph();
    // Check that the resulting graph matches the expected graph
    EXPECT_TRUE(ann->checkGraph(expected));
}
