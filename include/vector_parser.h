#ifndef VECTOR_PARSER_H
#define VECTOR_PARSER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

// Different file formats for vectors
enum FileFormat{
    FVECS, IVECS, BVECS
};

class VectorParser{
private:
    std::string m_file_path;
    FileFormat m_file_format;

    template <typename T>
    std::vector<std::vector<T>> parseVecs();

public:
    VectorParser(std::string file_path, FileFormat file_format) : m_file_path(file_path), m_file_format(file_format){}

    std::vector<std::vector<float>> parseFloats();
    std::vector<std::vector<int>> parseInts();
    std::vector<std::vector<unsigned char>> parseBytes();
};

#endif // vector_parser.h