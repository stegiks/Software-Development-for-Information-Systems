#include <gtest/gtest.h>
#include "utils_main.h"

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