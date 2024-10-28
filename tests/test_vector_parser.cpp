#include <gtest/gtest.h>
#include "vector_parser.h"

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
    std::string file_path = "test_floats.fvecs";
    std::vector<std::vector<float>> float_vectors = {
        {1.1, 2.1, 3.1},
        {4.1, 5.1, 6.1},
        {7.1, 8.1, 9.1}
    };

    createFile(file_path, float_vectors);

    VectorParser float_parser(file_path, FileFormat::FVECS);
    std::vector<std::vector<float>> parsed_vectors = float_parser.parseFloats();

    EXPECT_EQ(float_vectors, parsed_vectors);

    // Use remove and c_str to convert the string to a const char* and delete it
    std::remove(file_path.c_str());

    // Ints
    file_path = "test_ints.ivecs";
    std::vector<std::vector<int>> int_vectors = {
        {100, 200, 300},
        {1000, 2000, 3000},
        {10000, 20000, 30000}
    };

    createFile(file_path, int_vectors);

    VectorParser int_parser(file_path, FileFormat::IVECS);
    std::vector<std::vector<int>> parsed_int_vectors = int_parser.parseInts();

    EXPECT_EQ(int_vectors, parsed_int_vectors);

    std::remove(file_path.c_str());

    // Bytes
    file_path = "test_bytes.bvecs";
    std::vector<std::vector<unsigned char>> byte_vectors = {
        {'a', 'b', 'c'},
        {'d', 'e', 'f'},
        {'g', 'h', 'i'}
    };

    createFile(file_path, byte_vectors);

    VectorParser byte_parser(file_path, FileFormat::BVECS);
    std::vector<std::vector<unsigned char>> parsed_byte_vectors = byte_parser.parseBytes();

    EXPECT_EQ(byte_vectors, parsed_byte_vectors);

    std::remove(file_path.c_str());
}