#include "converter_json.h"

void ConverterJSON::check_config_file() {
    std::ifstream file("config.json");

    if(!file.is_open()) {
        throw std::runtime_error("config file is missing\n");
    }
   
    file >> config_parameters;
    file.close();

    if (!config_parameters.contains("config")) {
        throw std::invalid_argument("config file is empty");
    }
    else {
        source_files_paths = config_parameters.at("files");
    }
}


std::vector<std::string> ConverterJSON::GetTextDocuments() {
    
    for (const auto& file_path : source_files_paths) {
        std::ifstream file(file_path);

        if (!file.is_open()) {
            std::cout << "File not found: " << file_path << std::endl;
        }
        else {
            std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            file.close();
            source_files_contents.push_back(file_content); 
        }     
    }

    return source_files_contents;
}


int ConverterJSON::GetResponsesLimit() {
    int response_limit = 0;

    if (config_parameters.contains("config")) {
        response_limit = config_parameters.at("config").at("max_responses");
    }
    
    return response_limit;
}


std::vector<std::string> ConverterJSON::GetRequests() {
    std::ifstream request_file("requests.json");
    std::vector<std::string> requests_vector;
    
    if(request_file.is_open()) {
        request_file >> requests;
        requests_vector = requests.at("requests");
    }
    request_file.close();
    
    return requests_vector;
}


void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
    std::ofstream file_json("answers.json");
    nlohmann::json answers_to_write, request;
    std::string request_indx;
    
    for(size_t i = 0; i < answers.size();++i) {
        request_indx = "Request00" + std::to_string(i + 1);
    
        if(answers[i].size() == 0) {
            request[request_indx]["result"] = "false";
           
        }else {
            request[request_indx]["result"] = "true";
            
            for(size_t j = 0; j < answers[i].size();++j) { 
                nlohmann::json result_pair;
                result_pair["doc_id"] = answers[i][j].first;
                result_pair["rank"] = answers[i][j].second;
                request[request_indx]["relevance"] += result_pair;  
            } 
        }
    }
    answers_to_write["answers"] = request;
    file_json << answers_to_write;
    file_json.close();
}


ConverterJSON::ConverterJSON() {
    check_config_file();
}
