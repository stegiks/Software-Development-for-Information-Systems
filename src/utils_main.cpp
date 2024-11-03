#include "utils_main.h"
#include "ann.h"

std::string findExtension(const std::string& file_path){
    std::size_t pos = file_path.find_last_of(".");
    if(pos == std::string::npos){
        return "";
    }
    return file_path.substr(pos + 1);
}

bool validateExtension(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, const std::string& file_format){
    
    std::string extension_base = findExtension(file_path_base);
    std::string extension_query = findExtension(file_path_query);
    std::string extension_gt = findExtension(file_path_gt);
    
    if(extension_base != extension_query){
        std::cerr << RED << "Error : Base and query files have different extensions" << RESET << std::endl;
        return false;
    }

    if(extension_base != "ivecs" && extension_base != "fvecs" && extension_base != "bvecs"){
        std::cerr << RED << "Error : Base and Query files have an invalid extension" << RESET << std::endl;
        std::cerr << BLUE << "Valid extensions are : ivecs, fvecs, bvecs" << RESET << std::endl;
        return false;
    }

    if(file_format != extension_base){
        std::cerr << RED << "Error : File format and extension base are not the same" << RESET << std::endl;
        return false;
    }

    if(extension_gt != "ivecs"){
        std::cerr << RED << "Error : Ground truth file has an invalid extension" << RESET << std::endl;
        std::cerr << BLUE << "Valid extension is : ivecs" << RESET << std::endl;
        return false;
    }

    return true;
}

template <typename datatype>
std::vector<std::vector<datatype>> parseVecs(const std::string& file_path){
    // Open file in binary mode
    std::ifstream file(file_path, std::ios::binary);

    if(!file.is_open()){
        std::runtime_error("Could not open file " + file_path);
    }

    std::vector<std::vector<datatype>> vec;
    while(true){
        
        // Read the dimension of the vector
        int dim;
        file.read((char*)&dim, sizeof(int));
        if(!file) break;    // Break if we reached EOF

        // Read the vector
        std::vector<datatype> vec_temp(dim);
        file.read((char*)vec_temp.data(), dim * sizeof(datatype));
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

template <typename datatype>
void processing(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L){
    
    std::vector<std::vector<datatype>> base = parseVecs<datatype>(file_path_base);
    std::vector<std::vector<datatype>> query = parseVecs<datatype>(file_path_query);
    std::vector<std::vector<int>> gt = parseVecs<int>(file_path_gt);

    std::cout << GREEN << "Files parsed successfully" << RESET << std::endl;

    // Init ANN class and run Vamana algorithm
    ANN<datatype> ann(base, (size_t)R);

    std::cout << GREEN << "ANN class initialized successfully" << RESET << std::endl;

    ann.Vamana(alpha, L, R);

    std::cout << GREEN << "Vamana algorithm executed successfully" << RESET << std::endl;

    // For every query point, find the results and compare with ground truth
    int total_correct_guesses = 0;
    int total_gt_size = 0;

    for(std::size_t i = 0; i < query.size(); i++){
        int k = (int)gt[i].size();

        CompareVectors<datatype> compare(ann.node_to_point_map,query[i]);
        std::set<int, CompareVectors<datatype>> NNS(compare);
        std::unordered_set<int> Visited;

        // Call Greedy search to find the nearest neighbours
        ann.greedySearch(ann.getMedoid(), k, L, NNS, Visited, compare);

        // Search in the ground truth
        int correct = 0;
        for(const int& index : gt[i]){
            if(NNS.find(index) != NNS.end()){
                correct++;
            }
        }
        float accuracy = (float)correct / k * 100;
        std::cout << "Query " << i << " accuracy : " << accuracy << "%" << std::endl;

        total_correct_guesses += correct;
        total_gt_size += k; 
    }

    float total_accuracy = (float)total_correct_guesses / total_gt_size * 100;
    std::cout << "Total accuracy : " << total_accuracy << "%" << std::endl;
}

// Explicit instantiation of the parseVecs function
template std::vector<std::vector<float>> parseVecs<float>(const std::string& file_path);
template std::vector<std::vector<int>> parseVecs<int>(const std::string& file_path);
template std::vector<std::vector<unsigned char>> parseVecs<unsigned char>(const std::string& file_path);

// Explicit instantiation of the processing function
template void processing<float>(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L);
template void processing<int>(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L);
template void processing<unsigned char>(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L);