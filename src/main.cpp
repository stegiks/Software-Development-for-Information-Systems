#include "vector_parser.h"
#include "defs.h"

int main(int argc, char** argv){

    if(argc < 5){
        std::cout << RED << "Error : Not enough arguments" << RESET << std::endl;
        std::cout << BLUE << "Usage : "  << RESET << "./main <file_path_base> <file_path_query> <file_format> <a>" << std::endl;
        return 1;
    }

    // TODO : Process the command line arguments

    // Create a VectorParser object for float vectors
    VectorParser parser("./datasets/siftsmall/siftsmall_query.fvecs", FileFormat::FVECS);
    std::vector<std::vector<float>> vecs = parser.parseFloats();

    std::cout << "Number of vectors: " << vecs.size() << std::endl;
    // Print the first 10 vectors to test the parser
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < vecs[i].size(); j++){
            std::cout << vecs[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}