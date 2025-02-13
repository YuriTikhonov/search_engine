#include "SearchServer.h"


//std::vector<std::string> SearchServer::sort_query_words(std::vector<std::string>& words) {
  // Sort words in descending order based on their frequency
  //std::sort(words.begin(), words.end(), [](const std::string& a, const std::string& b) {
     // return freq_dictionary.at(a).size() > freq_dictionary.at(b).size();
 // });
 // return words;
//}


std::vector<std::string> SearchServer::create_unique_query_words(std::string& words) {
    std::unordered_set<std::string>query_set;
    std::vector<std::string> query_vec;
        int queries_count = 0;
        
        //количество запросов < 1000
        if(queries_count < 1000) {
            std::istringstream query_stream(words);        
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
    std::unordered_set<std::string> :: iterator itr;

    for (itr = query_set.begin(); itr != query_set.end(); itr++) {

       query_vec.push_back(*itr);
    }

    return query_vec;
} 