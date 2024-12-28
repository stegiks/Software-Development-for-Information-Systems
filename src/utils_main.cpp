#include "utils_main.h"
#include "ann.h"
#include <iomanip>
#include <filesystem>

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

    if(extension_base != "ivecs" && extension_base != "fvecs" && extension_base != "bvecs" && extension_base != "bin"){
        std::cerr   << "Error : Base vector file has an invalid extension" << RESET << std::endl;
        std::cerr << BLUE << "Valid extensions are : ivecs, fvecs, bvecs, bin" << RESET << std::endl;
        throw std::invalid_argument("Base vector file has an invalid extension");
        return false;
    }

    if(file_format != extension_base){
        std::cerr   << "Error : File format and extension base are not the same" << RESET << std::endl;
        throw std::invalid_argument("File format and extension base are not the same");
        return false;
    }

    if(extension_gt.empty()){
        return true;
    }
    
    if(extension_base != "bin"){
        if(extension_gt != "ivecs"){
            std::cerr   << "Error : Ground truth file has an invalid extension" << RESET << std::endl;
            std::cerr << BLUE << "Valid extension for provided arguments is : ivecs" << RESET << std::endl;
            throw std::invalid_argument("Ground truth file has an invalid extension");
            return false;
        }
    }
    else{
        if(extension_gt != "bin"){
            std::cerr   << "Error : Ground truth file has an invalid extension" << RESET << std::endl;
            std::cerr << BLUE << "Valid extension for provided arguments is : bin" << RESET << std::endl;
            throw std::invalid_argument("Ground truth file has an invalid extension");
            return false;
        }
    }

    return true;
}

// Function that calculates the ground truth vectors for the queries
template <typename datatype>
void calculateGroundTruth(const std::vector<std::vector<datatype>>& queries, 
                            const std::vector<std::vector<datatype>>& base_points,
                            std::vector<std::vector<std::pair<float, int>>>& ground_truth,
                            const std::vector<float>* query_category_values,
                            const std::vector<float>* base_category_values){

    // Preallocate memory
    ground_truth.clear();
    ground_truth.resize(queries.size());

    std::size_t n = queries.size();
    for(std::size_t i = 0; i < n; i++){
        const auto& query = queries[i];
        std::vector<std::pair<float, int>> points_for_x_filter;

        // Find the category value of the query
        float query_category_value = query_category_values == nullptr ? -1 : (*query_category_values)[i];

        // Calculate the distance of the query point to all the points in the base that have the same category value
        // If category value is -1, calculate the distance to all the points in the base
        std::size_t m = base_points.size();
        for(std::size_t j = 0; j < m; j++){
            if(query_category_value == -1 || (base_category_values != nullptr && (*base_category_values)[j] == query_category_value)){
                float distance = calculateDistance(query, base_points[j], query.size());
                points_for_x_filter.emplace_back(std::make_pair(distance, j));
            }
        }

        // Sort according to the distance
        std::sort(points_for_x_filter.begin(), points_for_x_filter.end(), 
        [](const std::pair<float, int>& a, const std::pair<float, int>& b){ // Lambda function
            return a.first < b.first;
        });

        // Keep maximum 100 points
        int size_gt = (int)std::min(points_for_x_filter.size(), (size_t)100);
        ground_truth[i].assign(points_for_x_filter.begin(), points_for_x_filter.begin() + size_gt);
    }
}

void processBinFormat(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L, 
    const std::string& file_path_load, const std::string& file_path_save, const std::string& algo, bool do_query){
    std::vector<std::vector<float>> base;
    std::vector<float> base_category_values;
    std::vector<std::vector<float>> queries;
    std::vector<float> query_category_values;

    parseDataVector(file_path_base, base_category_values, base);
    parseQueryVector(file_path_query, query_category_values, queries);

    std::vector<std::vector<int>> gt;

    // If the ground truth file is not provided, calculate the ground truth and save it to a file
    std::string file_name = file_path_gt;
    if(file_path_gt.empty()){

        // Create the file name for the calculated ground truth
        std::ostringstream file_name_stream;
        std::string algorithm_used = "filtered";
        std::string dataset_name = (base.size()<size_t(100000)) ? "_small" : "_large";
        file_name_stream << "./groundtruth/groundtruth" << dataset_name << "_" << algorithm_used << ".bin";
        file_name = file_name_stream.str();

        // Check if the file already exists and ground truth is already calculated
        if(!std::filesystem::exists(file_name)){
            // Calculate the ground truth
            std::cout << BLUE << "Calculating ground truth. This may take a while..." << RESET << std::endl;
            std::vector<std::vector<std::pair<float, int>>> temp_gt;
            calculateGroundTruth(queries, base, temp_gt, &query_category_values, &base_category_values);
        
            std::ofstream file(file_name, std::ios::binary);
            if(!file){
                throw std::runtime_error("Could not open file to save ground truth");
            }

            // Save the ground truth to the file
            for(const auto& query_gt : temp_gt){
                int dimension = (int)query_gt.size();
                file.write((char*)&dimension, sizeof(int));

                for(const auto& [distance, index] : query_gt){
                    file.write((char*)&index, sizeof(int));
                }
            }

            file.close();
        }
    }

    // Parse the ground truth from the file
    gt = parseVecs<int>(file_name);

    // Check if the files are parsed successfully
    if(base.empty() || queries.empty() || gt.empty()){
        throw std::runtime_error("Parsing failed and returned empty vectors");
    }

    std::cout << GREEN << "Files parsed successfully" << RESET << std::endl;

    // Init ANN class and run Vamana algorithm
    ANN<float> ann(base, base_category_values);

    // Open the file to write the graph
    if(file_path_load.empty()){
        if(algo == "stitch"){
            std::cout << BLUE << "Running stitched Vamana algorithm to create the graph" << RESET << std::endl;
            ann.stitchedVamana(alpha, L, (int)(R / 2), R, 0);
            std::cout << GREEN << "Stitched Vamana Graph executed successfully" << RESET << std::endl;
        }
        else{
            std::cout << BLUE << "Running filtered Vamana algorithm to create the graph" << RESET << std::endl;
            ann.filteredVamana(alpha, L, R, 5, 0);
            std::cout << GREEN << "Filtered Vamana Graph executed successfully" << RESET << std::endl;
        }
    }
    else{
        // Load the graph from the file
        ann.loadGraph(file_path_load);
        std::cout << GREEN << "Graph loaded successfully" << RESET << std::endl;

        // Run filtered find medoid to fill filter_to_start_node used in filteredGreedySearch
        ann.filteredFindMedoid(5);
    }

    if(do_query){
        int total_correct_guesses = 0;
        int total_gt_size = 0;

        for(std::size_t i = 0; i < queries.size(); i++){
            int k = (int)gt[i].size();

            CompareVectors<float> compare(ann.node_to_point_map, queries[i]);
            std::set<int, CompareVectors<float>> NNS(compare);
            std::unordered_set<int> Visited;

            // Call filtered Greedy
            if(query_category_values[i] == -1){
                ann.filteredGreedySearch(-1, k, L, -1, NNS, Visited, compare);
            }
            else{
                int start_node = ann.getStartNode(query_category_values[i]);
                if(start_node == -1) continue;
                ann.filteredGreedySearch(start_node, k, L, query_category_values[i], NNS, Visited, compare);
            }

            // Search in the ground truth
            int correct = 0;
            for(const int& index : gt[i]){
                if(NNS.find(index) != NNS.end()){
                    correct++;
                }
            }
            
            float recall = (float)correct / k * 100;
            std::cout << "Query " << i << " with category value " << query_category_values[i] << " recall : " << recall << "%" << std::endl;
            
            total_correct_guesses += correct;
            total_gt_size += k;
        }

        float total_recall = (float)total_correct_guesses / total_gt_size * 100;
        std::cout << BLUE << "Total recall : " << RESET << total_recall << "%" << std::endl;
    }

    if(!file_path_save.empty()){
        if (std::filesystem::exists(file_path_save)) {
            std::cout << YELLOW << "File already exists: " << file_path_save << RESET << std::endl;
        } 
        else {
            ann.saveGraph(file_path_save);
            std::cout << GREEN << "Graph saved successfully" << RESET << std::endl;
        }
    }
}

template <typename datatype>
void processVecFormat(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L,
     const std::string& file_path_load, const std::string& file_path_save, bool do_query){
    std::vector<std::vector<datatype>> base = parseVecs<datatype>(file_path_base);
    std::vector<std::vector<datatype>> query = parseVecs<datatype>(file_path_query);
    std::vector<std::vector<int>> gt;

    std::string file_name = file_path_gt;
    // If the ground truth file is not provided, calculate the ground truth and save it to a file
    if(file_path_gt.empty()){
        // Create the file name for the calculated ground truth
        std::ostringstream file_name_stream;
        std::string algorithm_used = "unfiltered";
        std::string dataset_name = (base.size()<size_t(100000)) ? "_small" : "_large";
        file_name_stream << "./groundtruth/groundtruth" << dataset_name << "_" << algorithm_used << ".bin";
        file_name = file_name_stream.str();

        // Check if the file already exists and ground truth is already calculated
        if(!std::filesystem::exists(file_name)){
            std::cout << BLUE << "Calculating ground truth. This may take a while..." << RESET << std::endl;
            std::vector<std::vector<std::pair<float, int>>> temp_gt;
            calculateGroundTruth(query, base, temp_gt);
            
            std::ofstream file(file_name, std::ios::binary);
            if(!file){
                throw std::runtime_error("Could not open file to save ground truth");
            }

            for(const auto& query_gt : temp_gt){
                int dimension = (int)query_gt.size();
                file.write((char*)&dimension, sizeof(int));

                for(const auto& [distance, index] : query_gt){
                    file.write((char*)&index, sizeof(int));
                }
            }

            file.close();
        }
    }
    
    // Parse the ground truth from the file
    gt = parseVecs<int>(file_name);
    
    // Check if the files are parsed successfully
    if(base.empty() || query.empty() || gt.empty()){
        throw std::runtime_error("Parsing failed and returned empty vectors");
    }

    std::cout << GREEN << "Files parsed successfully" << RESET << std::endl;

    // Init ANN class and run Vamana algorithm
    ANN<datatype> ann(base, (size_t)R);
    std::cout << GREEN << "ANN class initialized successfully" << RESET << std::endl;
    
    // Open the file to write the graph
    if(file_path_load.empty()){
        std::cout << BLUE << "Running Vamana algorithm to create the graph" << RESET << std::endl;
        ann.Vamana(alpha, L, R, false, false);
        std::cout << GREEN << "Vamana Graph executed successfully" << RESET << std::endl;
    }
    else{
        // Load the graph from the file
        ann.loadGraph(file_path_load);
        std::cout << GREEN << "Graph loaded successfully" << RESET << std::endl;
    }

    if(do_query){
        // For every query point, find the results and compare with ground truth
        int total_correct_guesses = 0;
        int total_gt_size = 0;

        for(std::size_t i = 0; i < query.size(); i++){
            int k = (int)gt[i].size();

            CompareVectors<datatype> compare(ann.node_to_point_map, query[i]);
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
        std::cout << BLUE << "Total recall : " << RESET << total_recall << "%" << std::endl;

    }
    
    if(!file_path_save.empty()){
        if(std::filesystem::exists(file_path_save)){
            std::cout << YELLOW << "File already exists: " << file_path_save << RESET << std::endl;
        }
        else{
            ann.saveGraph(file_path_save);
            std::cout << GREEN << "Graph saved successfully" << RESET << std::endl;
        }
    }
}

// Explicit instantiation of the processing function
template void processVecFormat<int>(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L, const std::string& file_path_load, const std::string& file_path_save, bool do_query);
template void processVecFormat<float>(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L, const std::string& file_path_load, const std::string& file_path_save, bool do_query);
template void processVecFormat<unsigned char>(const std::string& file_path_base, const std::string& file_path_query, const std::string& file_path_gt, float alpha, int R, int L, const std::string& file_path_load, const std::string& file_path_save, bool do_query);