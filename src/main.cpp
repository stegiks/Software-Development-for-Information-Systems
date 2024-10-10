#include "vector_parser.h"

int main(){
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