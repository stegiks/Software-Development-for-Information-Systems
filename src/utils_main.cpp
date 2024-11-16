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
        std::cerr   << "Error : Base and query files have different extensions" << RESET << std::endl;
        throw std::invalid_argument("Base and query files have different extensions");
        return false;
    }

    if(extension_base != "ivecs" && extension_base != "fvecs" && extension_base != "bvecs"){
        std::cerr   << "Error : Base and Query files have an invalid extension" << RESET << std::endl;
        std::cerr << BLUE << "Valid extensions are : ivecs, fvecs, bvecs" << RESET << std::endl;
        throw std::invalid_argument("Base and Query files have an invalid extension");
        return false;
    }

    if(file_format != extension_base){
        std::cerr   << "Error : File format and extension base are not the same" << RESET << std::endl;
        throw std::invalid_argument("File format and extension base are not the same");
        return false;
    }

    if(extension_gt != "ivecs"){
        std::cerr   << "Error : Ground truth file has an invalid extension" << RESET << std::endl;
        std::cerr << BLUE << "Valid extension is : ivecs" << RESET << std::endl;
        throw std::invalid_argument("Ground truth file has an invalid extension");
        return false;
    }

    return true;
}

template <typename datatype>
std::vector<std::vector<datatype>> parseVecs(const std::string& file_path){
    // Open file in binary mode
    std::ifstream file(file_path, std::ios::binary);

    if(!file.is_open()){
        throw std::runtime_error("Could not open file " + file_path);
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
            throw std::runtime_error("Error reading vector");
        }

        // Using std::move to avoid copying the vector 
        // because we are done with vec_temp
        vec.push_back(std::move(vec_temp));
    }

    file.close();
    return vec;
}

template <typename datatype>
void parseQueryVector(const std::string& path, std::vector<Query<datatype>>& queries){
    std::ifstream file(path, std::ios::binary);

    if(!file){
        throw std::runtime_error("Could not open file");
    }

    u_int32_t num_queries;
    file.read((char*)&num_queries, sizeof(u_int32_t));

    // We don't want to keep all the points because we don't care about timestamps queries.
    // So we don't care for query_id 2 and 3. That's why we don't preallocate memory for the points.
    for(u_int32_t i = 0; i < num_queries; i++){
        float query_id;
        float category_value;

        // Read first 8 bytes and then skip the next 8 bytes
        file.read((char*)&query_id, sizeof(float));
        file.read((char*)&category_value, sizeof(float));
        file.seekg(2 * sizeof(float), std::ios::cur);

        // Read the 100 dimension float vector
        std::vector<datatype> point(100);
        file.read((char*)point.data(), 100 * sizeof(datatype));

        // Use std::move to avoid copying the vector
        if(query_id == 0 || query_id == 1){
            Query<datatype> query;
            query.query_id = query_id;
            query.category_value = category_value;
            query.point = std::move(point);
            queries.push_back(std::move(query));
        }
    }

    file.close();
}

template <typename datatype>
void parseDataVector(const std::string& path, std::vector<float>& vec_with_category_values, std::vector<std::vector<datatype>>& vec_with_points){
    std::ifstream file(path, std::ios::binary);

    if(!file){
        throw std::runtime_error("Could not open file");
    }

    u_int32_t num_points;
    file.read((char*)&num_points, sizeof(u_int32_t));
    std::cout << "Number of points: " << num_points << std::endl;

    // Preallocate memory for the vectors, because we know the size
    vec_with_category_values.resize(num_points);
    // ! This may need a change because we may not know the dimension of the point vector.
    // ! In the datasets that we have, the dimension is always 100.
    vec_with_points.resize(num_points, std::vector<float>(100));

    for(u_int32_t i = 0; i < num_points; i++){
        // Categorical filter
        file.read((char*)&vec_with_category_values[i], sizeof(float));

        // Skip the 4 bytes used for timestamp
        file.seekg(4, std::ios::cur);

        // Read the 100 dimension float vector
        file.read((char*)vec_with_points[i].data(), 100 * sizeof(float));
    }

    file.close();
}

template <typename datatype>
void processing(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L){
    
    std::vector<std::vector<datatype>> base = parseVecs<datatype>(file_path_base);
    std::vector<std::vector<datatype>> query = parseVecs<datatype>(file_path_query);
    std::vector<std::vector<int>> gt = parseVecs<int>(file_path_gt);

    // Check if the files are parsed successfully
    if(base.empty() || query.empty() || gt.empty()){
        throw std::runtime_error("Parsing failed and returned empty vectors");
    }

    std::cout << GREEN << "Files parsed successfully" << RESET << std::endl;

    // Init ANN class and run Vamana algorithm
    ANN<datatype> ann(base, (size_t)R);

    std::cout << GREEN << "ANN class initialized successfully" << RESET << std::endl;

    ann.Vamana(alpha, L, R);

    std::cout << GREEN << "Vamana Graph executed successfully" << RESET << std::endl;

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
        float recall = (float)correct / k * 100;
        std::cout << "Query " << i << " recall : " << recall << "%" << std::endl;

        total_correct_guesses += correct;
        total_gt_size += k; 
    }

    float total_recall = (float)total_correct_guesses / total_gt_size * 100;
    std::cout << "Total recall : " << total_recall << "%" << std::endl;
}

// Explicit instantiation of the parseVecs function
template std::vector<std::vector<float>> parseVecs<float>(const std::string& file_path);
template std::vector<std::vector<int>> parseVecs<int>(const std::string& file_path);
template std::vector<std::vector<unsigned char>> parseVecs<unsigned char>(const std::string& file_path);

// Explicit instantiation of the processing function
template void processing<float>(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L);
template void processing<int>(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L);
template void processing<unsigned char>(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L);