#pragma once
#include "converter_json.h"
#include "InvertedIndex.h"
#include "windows.h"
#include "nlohmann/json.hpp"

struct RelativeIndex {
    size_t doc_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const {

        return (doc_id == other.doc_id && rank == other.rank);
    }
};


class SearchServer {
    InvertedIndex _index;
    public:
    
    SearchServer(const InvertedIndex& idx): _index(idx){
        std::cout << "Search Server created" << std::endl;
     };
   
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> result;

    for (const auto& query : queries_input) {
       
        std::vector<Entry> word_counts = _index.GetWordCount(query);
        std::vector<RelativeIndex> relative_indices;
        for (const auto& entry : word_counts) {
            RelativeIndex relative_index{entry.doc_id, entry.count};//narrowing conversion of '(size_t)entry.Entry::count' from 'size_t' {aka 'long long unsigned int'} to 'float'
            relative_indices.push_back(relative_index);
        }
        result.push_back(relative_indices);
    }
    return result;
};
 
};
