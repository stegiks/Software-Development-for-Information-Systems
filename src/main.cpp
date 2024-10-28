#include "vector_parser.h"
#include "defs.h"
#include "utils_main.h"
#include "ann.h"

int main(int argc, char** argv){

    if(argc < 8){
        std::cout << RED << "Error : Not enough arguments" << RESET << std::endl;
        std::cout << BLUE << "Usage : "  << RESET << "./main <file_path_base> <file_path_query> <extension> <file_path_gt> <alpha> <Regularity> <L(upper limit)>" << std::endl;
        return 1;
    }

    std::string file_path_base = argv[1];
    std::string file_path_query = argv[2];
    std::string file_format = argv[3];
    std::string file_path_gt = argv[4];
    float alpha = std::stof(argv[5]);
    int R = std::stoi(argv[6]);
    int L = std::stoi(argv[7]);

    if(!validateExtension(file_path_base, file_path_base, file_path_gt, file_format))
        return 1;
    
    if(file_format == "fvecs"){
        VectorParser parser_base(file_path_base, FileFormat::FVECS);
        VectorParser parser_query(file_path_query, FileFormat::FVECS);
        VectorParser parser_gt(file_path_gt, FileFormat::IVECS);

        std::vector<std::vector<float>> base = parser_base.parseFloats();
        std::vector<std::vector<float>> query = parser_query.parseFloats();
        std::vector<std::vector<int>> gt = parser_gt.parseInts();

        // Make Vamana Graph
        ANN<float> ann(base);
        ann.Vamana(alpha, L, R);

        // Test with query vectors using greedy search and ground truth vectors
        for(const auto& query_point : query){
            CompareVectors<float> compare(query_point);
            std::set<std::vector<float>, CompareVectors<float>> NNS(compare);
            std::set<std::vector<float>, CompareVectors<float>> Visited(compare);
            ann.greedySearch(query_point, query_point, 10, L, NNS, Visited, compare);
        }
    }
    else if(file_format == "ivecs"){
        VectorParser parser_base(file_path_base, FileFormat::IVECS);
        VectorParser parser_query(file_path_query, FileFormat::IVECS);
        VectorParser parser_gt(file_path_gt, FileFormat::IVECS);

        std::vector<std::vector<int>> base = parser_base.parseInts();
        std::vector<std::vector<int>> query = parser_query.parseInts();
        std::vector<std::vector<int>> gt = parser_gt.parseInts();

        ANN<int> ann(base);
    }
    else if(file_format == "bvecs"){
        VectorParser parser_base(file_path_base, FileFormat::BVECS);
        VectorParser parser_query(file_path_query, FileFormat::BVECS);
        VectorParser parser_gt(file_path_gt, FileFormat::IVECS);

        std::vector<std::vector<unsigned char>> base = parser_base.parseBytes();
        std::vector<std::vector<unsigned char>> query = parser_query.parseBytes();
        std::vector<std::vector<int>> gt = parser_gt.parseInts();

        ANN<unsigned char> ann(base);
    }

    std::cout << GREEN << "Vamana Graph created successfully" << RESET << std::endl;

    return 0;
}