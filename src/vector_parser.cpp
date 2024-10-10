#include "vector_parser.h"

std::vector<std::vector<float>> VectorParser::parseFloats(){
    
    if(m_file_format != FileFormat::FVECS){
        std::runtime_error("File format is not FVECS");
    }

    std::vector<std::vector<float>> vectors;
    // ! Parse the file and fill the vectors
    return vectors;
}

std::vector<std::vector<int>> VectorParser::parseInts(){
    
    if(m_file_format != FileFormat::IVECS){
        std::runtime_error("File format is not IVECS");
    }

    std::vector<std::vector<int>> vectors;
    // ! Parse the file and fill the vectors
    return vectors;
}

std::vector<std::vector<unsigned char>> VectorParser::parseBytes(){
    
    if(m_file_format != FileFormat::BVECS){
        std::runtime_error("File format is not BVECS");
    }

    std::vector<std::vector<unsigned char>> vectors;
    // ! Parse the file and fill the vectors
    return vectors;
}