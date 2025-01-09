#include <gtest/gtest.h>
#include "utils_ann.h"
#include "ann.h"
#include "parse.h"

// Create a file using the format described bellow
/*
    The float_vectors are stored in raw little endian. 
    Each vector takes 4+d*4 bytes for .fvecs and .ivecs formats, and 4+d bytes for .bvecs formats, 
    where d is the dimensionality of the vector and the first 4 bytes are the integer d.
*/
template <typename datatype>
void createFile(std::string& file_path, std::vector<std::vector<datatype>>& float_vectors){
    std::ofstream file(file_path, std::ios::binary);
    for(const auto& vec : float_vectors){
        // Write the dimension of the vector
        int dimension = (int)vec.size();
        file.write((char*)&dimension, sizeof(int));

        // Write the vector
        file.write((char*)vec.data(), dimension * sizeof(datatype));
    }
    file.close();
}

TEST(VectorParser, parseVec){
    // Floats
    {
        std::string file_path = "test_floats.fvecs";
        std::vector<std::vector<float>> float_vectors = {
            {1.1f, 2.1f, 3.1f},
            {4.1f, 5.1f, 6.1f},
            {7.1f, 8.1f, 9.1f}
        };

        createFile(file_path, float_vectors);

        std::vector<std::vector<float>> parsed_vectors = parseVecs<float>(file_path);

        EXPECT_EQ(float_vectors, parsed_vectors);
        std::remove(file_path.c_str());
    }

    // Ints
    {
        std::string file_path = "test_ints.ivecs";
        std::vector<std::vector<int>> int_vectors = {
            {100, 200, 300},
            {1000, 2000, 3000},
            {10000, 20000, 30000}
        };

        createFile(file_path, int_vectors);

        std::vector<std::vector<int>> parsed_vectors = parseVecs<int>(file_path);

        EXPECT_EQ(int_vectors, parsed_vectors);
        std::remove(file_path.c_str());
    }

    // Bytes
    {
        std::string file_path = "test_bytes.bvecs";
        std::vector<std::vector<unsigned char>> byte_vectors = {
            {'a', 'b', 'c'},
            {'d', 'e', 'f'},
            {'g', 'h', 'i'}
        };

        createFile(file_path, byte_vectors);

        std::vector<std::vector<unsigned char>> parsed_vectors = parseVecs<unsigned char>(file_path);

        EXPECT_EQ(byte_vectors, parsed_vectors);
        std::remove(file_path.c_str());
    }
}

TEST(UtilsANN, BasicHashFunctionality){
    std::vector<int> v1 = {1, 2, 3};
    std::vector<int> v2 = {1, 2, 3};
    std::vector<int> v3 = {1, 2, 4};
    std::vector<int> v4 = {1, 2, 3, 4};
    std::vector<int> v5 = {1, 2, 3, 4, 5};

    // Same vectors should hash to the same value
    // and different vectors should hash to a different one.
    VectorHash<int> vector_hasher;
    EXPECT_EQ(vector_hasher(v1), vector_hasher(v2));
    EXPECT_NE(vector_hasher(v1), vector_hasher(v3));
    EXPECT_NE(vector_hasher(v1), vector_hasher(v4));
    EXPECT_NE(vector_hasher(v1), vector_hasher(v5));
}

TEST(UtilsANN, DistanceTest){
    std::vector<float> v1 = {3.0, 4.0};
    std::vector<float> v2 = {6.0, 8.0};
    std::size_t dim = v1.size();

    // Test known distance
    EXPECT_FLOAT_EQ(calculateDistance(v1, v2, dim), 25.0);

    // Test zero distance
    std::vector<float> v3 = {3.0, 4.0};
    EXPECT_FLOAT_EQ(calculateDistance(v1, v3, dim), 0.0);
}

TEST(UtilsANN, VectorComparison){
    std::vector<int> base_vec = {10, 20, 30};

    std::vector<int> v1 = {15, 25, 35};
    std::vector<int> v2 = {4, 25, 35};

    std::vector<std::vector<int>> vec = {v1, v2};

    CompareVectors<int> vector_comparator(vec, base_vec);
    std::set<int, CompareVectors<int>> custom_set(vector_comparator);
    custom_set.insert(0);
    custom_set.insert(1);

    // v1(0) should be closer to base_vec than v2(1)
    EXPECT_EQ(*(custom_set.begin()), 0);
}



TEST(FilteredFindMedoid, BasicFunctionality){
    std::vector<std::vector<int>> points = {
        {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}, {16, 17, 18},
        {19, 20, 21}, {22, 23, 24}, {25, 26, 27}, {28, 29, 30}, {31, 32, 33}, {34, 35, 36},
        {37, 38, 39}, {40, 41, 42}, {43, 44, 45}, {46, 47, 48}, {49, 50, 51}, {52, 53, 54}
    };

    std::vector<float> filters = {
        1.0f, 2.0f, 1.0f, 3.0f, 1.0f, 2.0f,
        3.0f, 1.0f, 1.0f, 2.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 1.0f, 2.0f, 3.0f, 4.0f
    };

    ANN<int> ann(points, filters);
    ann.filteredFindMedoid();
    EXPECT_TRUE(ann.checkFilteredFindMedoid(5));
}