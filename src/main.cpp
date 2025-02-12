#include "converter_json.h"
#include "SearchServer.h"
#include "InvertedIndex.h"
#include <thread>
#include <mutex>
#include <unordered_set>


int main() {
    ConverterJSON converter;
    InvertedIndex* inverted_index = new InvertedIndex(*inverted_index);
    SearchServer server(*inverted_index);
    /* 
    try {
        inverted_index->UpdateDocumentBase(converter.GetTextDocuments());
    }
    catch (const std::exception& e) {
        std::cout << "Caution! File is missing " << e.what();
        Sleep(int(5000)); //записать ошибку в лог
    }
    */
    std::cout << "response_lim: " << converter.GetResponsesLimit() << std::endl;
    std::vector<std::string> queries = converter.GetRequests();
    //inverted_index->printIndex();
    /*
    std::unordered_set<std::string>query_set;
    
    for(auto it : queries) {
        int queries_count = 0;
        
        //количество запросов < 1000
        if(queries_count < 1000) {
            std::istringstream query_stream(it);        
            int query_word_count = 0;

            while(query_stream) {
                std::string word;
                query_stream >> word;
            
                // количество слов в запросе от 1 до 10
                if(word != "" && query_word_count < 10) {
                    query_set.insert(word);
                    query_word_count++;
                }  
            }
        }
    }
   
    std::vector<std::vector<Entry>> result;
    std::unordered_set<std::string> :: iterator itr;
    int count = 0;
    
    for (itr = query_set.begin(); itr != query_set.end(); itr++) {
        std::vector<Entry> word_count = inverted_index->GetWordCount(*itr);
        result.push_back(word_count);

        // std::cout <<"itr# " << count << "Words count for: " << *itr << " : ";
        for(auto& it: word_count) {
       // std::cout << "(" << it.doc_id << ", " << it.count << ") ";
        }
       // std::cout << std::endl;
        count++;
    }
    //inverted_index->printIndex();
     */
     std::vector<std::vector<RelativeIndex>> relative_indx_vec = server.search(queries);
     
    delete inverted_index;
    return 0;
}
