#include "parse.h"

void parseDataVector(const std::string& path, std::vector<float>& vec_with_category_values, std::vector<std::vector<float>>& vec_with_points){
    std::ifstream file(path, std::ios::binary);

    if(!file){
        throw std::runtime_error("Could not open file");
    }

    u_int32_t num_points;
    file.read((char*)&num_points, sizeof(u_int32_t));
    std::cout << "Number of points: " << num_points << std::endl;

    // Preallocate memory for the vectors, because we know the size
    vec_with_category_values.resize(num_points);
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

void parseQueryVector(const std::string& path, std::vector<float>& query_filters, std::vector<std::vector<float>>& query_points){
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
        std::vector<float> point(100);
        file.read((char*)point.data(), 100 * sizeof(float));

        // Use std::move to avoid copying the vector
        if(query_id == 0 || query_id == 1){
            query_filters.push_back(category_value);
            query_points.push_back(std::move(point));
        }
    }

    file.close();
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

// Explicit instantiation of the parseVecs function
template std::vector<std::vector<float>> parseVecs<float>(const std::string& file_path);
template std::vector<std::vector<int>> parseVecs<int>(const std::string& file_path);
template std::vector<std::vector<unsigned char>> parseVecs<unsigned char>(const std::string& file_path);
