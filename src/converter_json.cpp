#include "converter_json.h"


void ConverterJSON::check_config_file() {
    std::ifstream file("config.json");
    
    if(!file.is_open()) {
        throw std::range_error("config file is missing");
    }

    file >> config_parameters;
    file.close();

    if (!config_parameters.contains("config")) {
        throw std::invalid_argument("config file is empty");
    }
    else {
        std::cout << config_parameters.at("config").at("name") << " started successfully" << std::endl;
        source_files_paths = config_parameters.at("files");
    }
}


std::vector<std::string> ConverterJSON::GetTextDocuments() {
    
    for (const auto& file_path : source_files_paths) {
        std::ifstream file(file_path);

        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + file_path);
        }
        else {
            std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            source_files_contents.push_back(file_content);
           // std::string lines;
           // std::string line;
            
           // while(std::getline(file, line)) {
            //    lines += line;
           // }
           // source_files_contents.push_back(lines);    
            file.close();   
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


void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>>answers) {

}


ConverterJSON::ConverterJSON() {
    try {
        check_config_file();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what();
        Sleep(int(5000));
    }   
}
