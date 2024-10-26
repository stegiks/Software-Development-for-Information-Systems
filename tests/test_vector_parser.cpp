#include <gtest/gtest.h>
#include "vector_parser.h"

// Create a file using the format described bellow
/*
    The vectors are stored in raw little endian. 
    Each vector takes 4+d*4 bytes for .fvecs and .ivecs formats, and 4+d bytes for .bvecs formats, 
    where d is the dimensionality of the vector and the first 4 bytes are the integer d.
*/
template <typename datatype>
void createFile(std::string& file_path, std::vector<std::vector<datatype>>& vectors){
    std::ofstream file(file_path, std::ios::binary);
    for(const auto& vec : vectors){
        // Write the dimension of the vector
        int dimension = (int)vec.size();
        file.write((char*)&dimension, sizeof(int));

        // Write the vector
        file.write((char*)vec.data(), dimension * sizeof(datatype));
    }
    file.close();
}

TEST(VectorParser, parseFloatVec){
    std::string file_path = "test.fvecs";
    std::vector<std::vector<float>> vectors = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0}
    };

    createFile(file_path, vectors);

    VectorParser parser(file_path, FileFormat::FVECS);
    std::vector<std::vector<float>> parsed_vectors = parser.parseFloats();

    EXPECT_EQ(vectors, parsed_vectors);

    // Use remove and c_str to convert the string to a const char* and delete it
    std::remove(file_path.c_str());
}