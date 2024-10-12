#include "vector_parser.h"
#include "defs.h"
#include "utils_ann.h"

int main(int argc, char** argv){

    if(argc < 5){
        std::cout << RED << "Error : Not enough arguments" << RESET << std::endl;
        std::cout << BLUE << "Usage : "  << RESET << "./main <file_path_base> <file_path_query> <file_format> <a>" << std::endl;
        return 1;
    }

    // TODO : Process the command line arguments
    std::string file_path_base = argv[1];
    std::string file_path_query = argv[2];
    std::string file_path_gt = argv[3];

    std::string extension_base = findExtension(file_path_base);
    std::string extension_query = findExtension(file_path_query);
    std::string extension_gt = findExtension(file_path_gt);

    if(!validateExtension(extension_base, extension_query, extension_gt))
        return 1;
    
    if(extension_base == "fvecs"){
        VectorParser parser_base(file_path_base, FileFormat::FVECS);
        VectorParser parser_query(file_path_query, FileFormat::FVECS);
        VectorParser parser_gt(file_path_gt, FileFormat::IVECS);

        std::vector<std::vector<float>> base = parser_base.parseFloats();
        std::vector<std::vector<float>> query = parser_query.parseFloats();
        std::vector<std::vector<int>> gt = parser_gt.parseInts();

        // TODO : Call function for example :
        // template <typename datatype>
        // void buildGraph(std::vector<std::vector<datatype>> base, std::vector<std::vector<datatype>> query, std::vector<std::vector<int>> gt, int a);
    }
    else if(extension_base == "ivecs"){
        VectorParser parser_base(file_path_base, FileFormat::IVECS);
        VectorParser parser_query(file_path_query, FileFormat::IVECS);
        VectorParser parser_gt(file_path_gt, FileFormat::IVECS);

        std::vector<std::vector<int>> base = parser_base.parseInts();
        std::vector<std::vector<int>> query = parser_query.parseInts();
        std::vector<std::vector<int>> gt = parser_gt.parseInts();

        // TODO : Call function for example :
        // template <typename datatype>
        // void buildGraph(std::vector<std::vector<datatype>> base, std::vector<std::vector<datatype>> query, std::vector<std::vector<int>> gt, int a);
    }
    else if(extension_base == "bvecs"){
        VectorParser parser_base(file_path_base, FileFormat::BVECS);
        VectorParser parser_query(file_path_query, FileFormat::BVECS);
        VectorParser parser_gt(file_path_gt, FileFormat::IVECS);

        std::vector<std::vector<unsigned char>> base = parser_base.parseBytes();
        std::vector<std::vector<unsigned char>> query = parser_query.parseBytes();
        std::vector<std::vector<int>> gt = parser_gt.parseInts();

        // TODO : Call function for example :
        // template <typename datatype>
        // void buildGraph(std::vector<std::vector<datatype>> base, std::vector<std::vector<datatype>> query, std::vector<std::vector<int>> gt, int a);
    }

    return 0;
}