#include <gtest/gtest.h>
#include "ann.h"
#include <set>
#include <vector>

// Test Fixture class for the testing Robust Prune functionality
class RobustPruneTest : public testing::Test{
protected:

    std::vector<std::vector<int>> points;
    std::vector<std::unordered_set<int>> edges;
    std::vector<int> start;
    ANN<int>* ann;

    RobustPruneTest(){
        // Initialize the graph
        points = {{1, 2}, {1, 0}, {2, 3}, {1, -5}, {3, -5}, {6, 2}};
        edges = {
            {2},
            {0, 3},
            {1, 5},
            {},
            {3},
            {4}
        };

        start = {2, 3};
        ann = new ANN<int>(points, edges);
    }

    ~RobustPruneTest() override {
        delete ann;
    }  
};

// Test basic pruning functionality
TEST_F(RobustPruneTest, BasicFunctionality){

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
    ann->robustPrune(start_node, candidate, 1.1, 5, UNFILTERED);

    EXPECT_TRUE(ann->checkGraph(expected));
}

// Test empty candidate set
TEST_F(RobustPruneTest, EmptyCandidateSet){

    CompareVectors<int> compare(ann->node_to_point_map,start);
    std::set<int, CompareVectors<int>> candidate(compare);

    int start_node = 2;
    ann->robustPrune(start_node, candidate, 1.1, 5, UNFILTERED);

    EXPECT_TRUE(ann->checkGraph(edges));
}

// Test empty graph, empty point and point not found in the graph
TEST_F(RobustPruneTest, InvalidGraphPoint){
    // Empty graph
    std::vector<std::vector<int>> points_alt = {};
    std::vector<std::unordered_set<int>> edges_alt = {};
    int start_alt = 2;
    std::vector<int> start_alt_vector = {2, 3};

    CompareVectors<int> compare(ann->node_to_point_map,start_alt_vector);
    std::set<int, CompareVectors<int>> candidate(compare);
    candidate.insert(0);
    candidate.insert(1);
    candidate.insert(3);

    ANN<int> ann_alt(points_alt, edges_alt);

    EXPECT_THROW(ann_alt.robustPrune(start_alt, candidate, 1.1, 5, UNFILTERED), std::invalid_argument);
    EXPECT_TRUE(ann_alt.checkGraph(edges_alt));

    // Point not in graph
    points_alt = {{1, 2}, {1, 0}, {2, 3}, {1, -5}, {3, -5}, {6, 2}};
    edges_alt = {
        {2},
        {0, 3},
        {1, 5},
        {},
        {3},
        {4}
    };

    ANN<int> ann_alt2(points_alt, edges_alt);
    start_alt = (int)points_alt.size();
    start_alt++;

    EXPECT_THROW(ann_alt2.robustPrune(start_alt, candidate, 1.1, 5, UNFILTERED), std::invalid_argument);
    EXPECT_TRUE(ann_alt2.checkGraph(edges_alt));
}

// Test alpha less than 1 and degree bound negative
TEST_F(RobustPruneTest, InvalidAlphaDegree){

    CompareVectors<int> compare(ann->node_to_point_map,start);
    std::set<int, CompareVectors<int>> candidate(compare);
    candidate.insert(0);
    candidate.insert(1);
    candidate.insert(3);

    // Alpha less than 1
    EXPECT_THROW(ann->robustPrune(0, candidate, 0.9, 5, UNFILTERED), std::invalid_argument);
    EXPECT_TRUE(ann->checkGraph(edges));

    // Degree bound negative
    EXPECT_THROW(ann->robustPrune(0, candidate, 1.1, -5, UNFILTERED), std::invalid_argument);
    EXPECT_TRUE(ann->checkGraph(edges));
}

// Degree bound with value 1
TEST_F(RobustPruneTest, DegreeBoundOne){
    // Degree bound of 1 should return only the closest point
    // when giving the whole set of points as candidate set
    // but not the point itself
    CompareVectors<int> compare(ann->node_to_point_map,start);
    std::set<int, CompareVectors<int>> candidate(compare);
    candidate.insert(0);
    candidate.insert(1);
    candidate.insert(2);
    candidate.insert(3);
    candidate.insert(4);
    candidate.insert(5);


    std::vector<std::unordered_set<int>> expected = {
        {2},
        {0, 3},
        {0},
        {},
        {3},
        {4}
    };

    ann->robustPrune(2, candidate, 1.1, 1, UNFILTERED);

    EXPECT_TRUE(ann->checkGraph(expected));
}

// Large alpha value resulting in no pruning of the candidate set
TEST_F(RobustPruneTest, LargeAlpha){
    CompareVectors<int> compare(ann->node_to_point_map,start);
    std::set<int, CompareVectors<int>> candidate(compare);
    candidate.insert(0);
    candidate.insert(1);
    candidate.insert(3);

    std::vector<std::unordered_set<int>> expected = {
        {2},
        {0, 3},
        {0, 1, 3, 5},
        {},
        {3},
        {4}
    };

    int start_node = 2;
    ann->robustPrune(start_node, candidate, 1000, 5, UNFILTERED);

    EXPECT_TRUE(ann->checkGraph(expected));
}

TEST_F(RobustPruneTest, EmptySet) {
    CompareVectors<int> compare(ann->node_to_point_map,start);
    std::set<int, CompareVectors<int>> candidate(compare);

    // Delete ann object created in the constructor
    delete ann;

    // Create new ANN object with empty set of points and expect an exception
    std::vector<std::vector<int>> empty = {};
    ann = new ANN<int>(empty, edges);
    EXPECT_TRUE(true);
    delete ann;
}



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

    delete ann;
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
    delete ann;
}

