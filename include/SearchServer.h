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
    std::vector<std::string> sort_query_words(std::vector<std::string>& words);
    std::vector<std::string> create_unique_query_words(std::string& words);
         
    public:
    
    
    SearchServer( InvertedIndex idx): _index(idx) {
        std::cout << "Search Server created"  <<std::endl;
    };
   
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input) {
        std::vector<std::vector<RelativeIndex>> result;
        std::vector<RelativeIndex> relative_indixes;

        ConverterJSON converter;

        try {
            _index.UpdateDocumentBase(converter.GetTextDocuments());
        }
        catch (const std::exception& e) {
            std::cout << "Caution! File is missing " << e.what(); //записать ошибку в лог
        }
     
        int i = 0;

        for (auto query : queries_input) {
            std::vector<std::string> query_set = create_unique_query_words(query);
       
            std::cout << "i " << i << ": ";

            for(auto it : query_set) {
                std::cout << " " << it;
                std::vector<Entry> word_counts = _index.GetWordCount(it);

                for(auto& e : word_counts) {
                    std::cout <<" {" << e.doc_id << "," << e.count << "}";
                }
            }
            std::cout  << std::endl;
       
            // std::vector<Entry> word_counts = _index.GetWordCount("milk"); //отправлять надо отдельное слово и ожидать паравозик данных
            // std::cout << "size::: " << word_counts.size() << std::endl;
            // for(auto& e : word_counts) {
            // std::cout <<"e_counts: " << e.doc_id << " " << e.count << std::endl;
            //}
        
            /*
            for (const auto& entry : word_counts) {
                RelativeIndex relative_index{entry.doc_id, entry.count};//narrowing conversion of '(size_t)entry.Entry::count' from 'size_t' {aka 'long long unsigned int'} to 'float'
                relative_indixes.push_back(relative_index);
            }
            result.push_back(relative_indixes);
            */
            i++;
        }

        return result;
 }; 
};
