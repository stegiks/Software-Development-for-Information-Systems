#include "vector_parser.h"

// Private method to parse vectors of any type
template <typename T>
std::vector<std::vector<T>> VectorParser::parseVecs(){
    // Open file in binary mode
    std::ifstream file(this->m_file_path, std::ios::binary);

    if(!file.is_open()){
        std::runtime_error("Could not open file " + this->m_file_path);
    }

    std::vector<std::vector<T>> vec;
    while(true){
        
        // Read the dimension of the vector
        int dim;
        file.read((char*)&dim, sizeof(int));
        if(!file) break;    // Break if we reached EOF

        // Read the vector
        std::vector<T> vec_temp(dim);
        file.read((char*)vec_temp.data(), dim * sizeof(T));
        if(!file){
            std::runtime_error("Error reading vector");
        }

        // Using std::move to avoid copying the vector 
        // because we are done with vec_temp
        vec.push_back(std::move(vec_temp));
    }

    file.close();
    return vec;
}

// Public methods to parse vectors of specific types
std::vector<std::vector<float>> VectorParser::parseFloats(){
    if(m_file_format != FileFormat::FVECS){
        std::runtime_error("File format is not FVECS");
    }

    return this->parseVecs<float>();
}

std::vector<std::vector<int>> VectorParser::parseInts(){
    if(m_file_format != FileFormat::IVECS){
        std::runtime_error("File format is not IVECS");
    }
    return this->parseVecs<int>();
}

std::vector<std::vector<unsigned char>> VectorParser::parseBytes(){ 
    if(m_file_format != FileFormat::BVECS){
        std::runtime_error("File format is not BVECS");
    }
    return this->parseVecs<unsigned char>();
}