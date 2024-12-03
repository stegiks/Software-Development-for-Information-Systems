#include <gtest/gtest.h>
#include "ann.h"

// Basic test functionallity for the filtered greedy search algorithm
TEST(FilteredGreedySearch, BasicFilteredSearch){
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18}};
    // Filters
    std::vector<float> filters = {1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f};
    std::vector<std::unordered_set<int>> edges = {
        {1, 2},
        {0, 3},
        {0, 4},
        {1, 5},
        {2, 5},
        {3, 4}
    };

    // Init ANN class and parameters for filtered greedy search
    ANN<int> ann(points, edges, filters);
    std::vector<int> query_vector = {6, 6, 6};
    int start_node = 0;
    int k = 2;
    int upper_limit = 3;
    float filter_query_value = 1.0f;

    CompareVectors<int> compare(ann.node_to_point_map, query_vector);
    std::set<int, CompareVectors<int>> NNS(compare);
    std::unordered_set<int> Visited;
    NNS.insert(start_node);

    ann.filteredGreedySearch(start_node, k, upper_limit, filter_query_value, NNS, Visited, compare);
    std::set<int, CompareVectors<int>> expected(compare);
    expected.insert(0);
    expected.insert(2);
    EXPECT_EQ(NNS, expected);

    // Different graph edges may result in different nearest neighbours
    edges = {
        {1, 4},
        {0, 3, 5},
        {1, 3},
        {2, 4, 5},
        {0, 3, 5},
        {1, 2}
    };

    ANN<int> ann2(points, edges, filters);
    NNS.clear();
    Visited.clear();
    NNS.insert(start_node);
    ann2.filteredGreedySearch(start_node, k, upper_limit, filter_query_value, NNS, Visited, compare);

    expected.clear();
    expected.insert(0);
    expected.insert(4);
    EXPECT_EQ(NNS, expected);
}

// Unfiltered search by passing -1 as start node
TEST(FilteredGreedySearch, UnfilteredSearch){
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18}};
    // Filters
    std::vector<float> filters = {1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f};
    std::vector<std::unordered_set<int>> edges = {
        {1, 2},
        {0, 3},
        {0, 4},
        {1, 5},
        {2, 5},
        {3, 4}
    };

    // Init ANN class and parameters for filtered greedy search
    ANN<int> ann(points, edges, filters);
    std::vector<int> query_vector = {6, 6, 6};
    int start_node = -1;
    int k = 2;
    int upper_limit = 3;
    float filter_query_value = -1.0f;

    CompareVectors<int> compare(ann.node_to_point_map, query_vector);
    std::set<int, CompareVectors<int>> NNS(compare);
    std::unordered_set<int> Visited;

    // Unfiltered search should return the closest points no matter the filter value
    std::unordered_map<float, int> filter_to_start_node = {{1.0f, 0}, {2.0f, 1}};
    ann.fillFilterToStartNode(filter_to_start_node);
    ann.filteredGreedySearch(start_node, k, upper_limit, filter_query_value, NNS, Visited, compare);
    std::set<int, CompareVectors<int>> expected(compare);
    expected.insert(1);
    expected.insert(2);
    EXPECT_EQ(NNS, expected);
}

// Test for different errors
TEST(FilteredGreedySearch, ErrorChecking){
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18}};
    // Filters
    std::vector<float> filters = {1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f};
    std::vector<std::unordered_set<int>> edges = {
        {1, 2},
        {0, 3},
        {0, 4},
        {1, 5},
        {2, 5},
        {3, 4}
    };

    // Init ANN class and parameters for filtered greedy search
    ANN<int> ann(points, edges, filters);
    std::vector<int> query_vector = {6, 6, 6};
    int start_node = 0;
    int k = 2;
    int upper_limit = 3;
    
    // ! Filter value that doesn't exist
    float filter_query_value = 3.0f;

    CompareVectors<int> compare(ann.node_to_point_map, query_vector);
    std::set<int, CompareVectors<int>> NNS(compare);
    std::unordered_set<int> Visited;
    NNS.insert(start_node);

    // Error checking for filter value that doesn't exist
    ann.filteredGreedySearch(start_node, k, upper_limit, filter_query_value, NNS, Visited, compare);

    // NNS will only have start node with this implementation
    std::set<int, CompareVectors<int>> expected(compare);
    expected.insert(start_node);
    EXPECT_EQ(NNS, expected);

    // ! Invalid start node
    start_node = -2;
    NNS.clear();
    NNS.insert(start_node);
    EXPECT_THROW(ann.filteredGreedySearch(start_node, k, upper_limit, filter_query_value, NNS, Visited, compare), std::invalid_argument);

    // ! Empty graph
    std::vector<std::vector<int>> empty_points = {};
    std::vector<float> empty_filters = {};
    std::vector<std::unordered_set<int>> empty_edges = {};

    ANN<int> ann_empty(empty_points, empty_edges, empty_filters);
    std::vector<int> empty_query_vector = {6, 6, 6};
    EXPECT_THROW(CompareVectors<int> compare_empty(ann_empty.node_to_point_map, empty_query_vector), std::invalid_argument);

    // ! Empty query vector
    EXPECT_THROW(CompareVectors<int> compare_empty(ann.node_to_point_map, {}), std::invalid_argument);

    // ! Dimension mismatch in query vector and nodes in the graph
    std::vector<int> query_vector_mismatch = {1, 2, 3, 4};
    EXPECT_THROW(CompareVectors<int> compare_mismatch(ann.node_to_point_map, query_vector_mismatch), std::invalid_argument);
}

// Different k values for the filtered greedy search
TEST(FilteredGreedySearch, DifferentKValues){
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}};
    std::vector<float> filters = {1.0f, 1.0f};
    std::vector<std::unordered_set<int>> edges = {{1}, {0}};

    ANN<int> ann(points, edges, filters);
    int start_node = 0;
    std::vector<int> query_vector = {6, 6, 6};
    float filter_query_value = 1.0f;

    CompareVectors<int> compare(ann.node_to_point_map, query_vector);
    std::set<int, CompareVectors<int>> NNS(compare);
    std::unordered_set<int> Visited;
    NNS.insert(start_node);

    // k greater than number of nodes
    int k = 3;
    int upper_limit = 3;

    ann.filteredGreedySearch(start_node, k, upper_limit, filter_query_value, NNS, Visited, compare);
    std::set<int, CompareVectors<int>> expected(compare);
    expected.insert(0);
    expected.insert(1);
    EXPECT_EQ(NNS, expected);

    NNS.clear();
    Visited.clear();
    expected.clear();
    NNS.insert(start_node);

    // k equal to 0
    k = 0;
    ann.filteredGreedySearch(start_node, k, upper_limit, filter_query_value, NNS, Visited, compare);
    EXPECT_TRUE(NNS.empty());

    NNS.clear();
    Visited.clear();

    // ! Upper limit less than k
    k = 2;
    upper_limit = 1;
    NNS.insert(start_node);
    EXPECT_THROW(ann.filteredGreedySearch(start_node, k, upper_limit, filter_query_value, NNS, Visited, compare), std::invalid_argument);
}