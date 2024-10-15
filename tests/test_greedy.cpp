#include <gtest/gtest.h>
#include "ann.h"

// Test 1
TEST(GreedySearch, Test1){
    std::vector<std::vector<int>> points = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18}};
    ANN<int> ann(points);
    std::vector<int> start_node = {1, 2, 3};
    std::vector<int> query_node = {6, 6, 6};
    int k = 2;
    int upper_limit = 2;
    std::set<std::vector<int>> result = ann.greedySearch(start_node, query_node, k, upper_limit);
    std::set<std::vector<int>> expected = {{4, 5, 6}, {7, 8, 9}};

    ASSERT_EQ(result, expected);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}