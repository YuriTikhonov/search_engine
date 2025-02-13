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
    std::vector<std::string> sort_query_words(const std::string& word, int word_frequency);// доработать структуру!!
    std::vector<std::string> create_unique_query_words(std::string& words);
         
    public:
    
    
    SearchServer( InvertedIndex idx): _index(idx) {
        std::cout << "Search Server created"  <<std::endl;
    };
   
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input) {
        std::vector<std::vector<RelativeIndex>> result;
        std::vector<RelativeIndex> relative_indixes;
        std::vector<std::string> sorted_words;

        ConverterJSON converter;

        try {
            _index.UpdateDocumentBase(converter.GetTextDocuments());
        }
        catch (const std::exception& e) {
            std::cout << "Caution! File is missing " << e.what(); //записать ошибку в лог
        }
     
        int i = 0;
        int frequency = 0;
        typedef std::pair<std::string,int> word_frec_pair;
        std::vector<word_frec_pair> pair_vec;
        

        for (auto query : queries_input) {
            std::vector<std::string> query_set = create_unique_query_words(query);
       
          

            for(auto it : query_set) {
             
                std::vector<Entry> word_counts = _index.GetWordCount(it);
                
              
                for(auto& e : word_counts) {
                 
                    frequency += e.count;
                }
            
                pair_vec.push_back(std::make_pair(it,frequency));

                 std::sort(pair_vec.begin(),pair_vec.end(),
                  [](auto &left, auto &right)
                  {
                   
                      return left.second < right.second;
                  });
                  
            }
           
            /*
            for (const auto& entry : word_counts) {
                RelativeIndex relative_index{entry.doc_id, entry.count};//narrowing conversion of '(size_t)entry.Entry::count' from 'size_t' {aka 'long long unsigned int'} to 'float'
                relative_indixes.push_back(relative_index);
            }
            result.push_back(relative_indixes);
            */
            i++;
        }

         std::cout << "s--------------------------------" << std::endl;
            for(auto& vec: pair_vec) {
                std::cout << " " << vec.first << " " << vec.second << std::endl;
            }
            std::cout << "e--------------------------------" << std::endl;

        return result;
 }; 
};
