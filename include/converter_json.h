#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "windows.h"

#include "nlohmann/json.hpp"
#include "InvertedIndex.h"
#include "SearchServer.h"


class ConverterJSON {
    void check_config_file();
    std::vector<std::string> source_files_paths;
    std::vector<std::string> source_files_contents;
    nlohmann::json config_parameters;
    nlohmann::json requests;

    public:

    ConverterJSON();// = default;

    std::vector<std::string> GetTextDocuments();

    int GetResponsesLimit();

    std::vector<std::string> GetRequests();

    void putAnswers(std::vector<std::vector<std::pair<int, float> >> answers);
};
