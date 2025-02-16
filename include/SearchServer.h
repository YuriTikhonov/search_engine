#pragma once
#include "converter_json.h"
#include "InvertedIndex.h"
#include "windows.h"
#include "nlohmann/json.hpp"
#include <unordered_set>

struct RelativeIndex {
    size_t doc_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const {

        return (doc_id == other.doc_id && rank == other.rank);
    }
};


class SearchServer {
    InvertedIndex _index;
    std::vector<size_t> docs_ids;
    std::vector<std::string> create_unique_query_words(std::string& words);
    std::vector<RelativeIndex> calculate_relevance(const std::vector<size_t> &docList,
        const std::vector<std::string> &request);
    void printResult(const std::vector<std::vector<RelativeIndex>> &answers);
    

    public:
    SearchServer( InvertedIndex idx);
   
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
};
