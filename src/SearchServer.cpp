#include "SearchServer.h"


//std::vector<std::string> SearchServer::sort_query_words(std::vector<std::string>& words) {
  // Sort words in descending order based on their frequency
  //std::sort(words.begin(), words.end(), [](const std::string& a, const std::string& b) {
     // return freq_dictionary.at(a).size() > freq_dictionary.at(b).size();
 // });
 // return words;
//}
 SearchServer::SearchServer( InvertedIndex idx): _index(idx) {
        std::cout << "Search Server created"  <<std::endl;
    };

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
std::vector<RelativeIndex>  SearchServer::calculate_relevance(const std::vector<size_t> &docList,
    const std::vector<std::string> &request) {
    std::vector<RelativeIndex> result;
    size_t absoluteRelevance = 0;
    
   // size_t relevance = 0.0f;??
    for (auto &i: docList) {
      size_t relevance = 0.0f;  
        std::cout << "i: " << i << " ";
        for (auto &word: request) {
            std::vector<Entry>temp = _index.GetWordCount(word);  // empty returned
          // std::cout << " word:  " << word;
            for(auto& w : temp) {
                //std::cout <<"{" << w.doc_id << ", " << w.count << "}";
                if (w.doc_id == i)
                 
                    relevance += w.count;
                     std::cout <<"w.count: " << w.count << " relevance: " << relevance << std::endl;
            } 
            //absoluteRelevance += relevance;
          }
         //std::cout << std::endl;
        if(relevance > absoluteRelevance)
           absoluteRelevance = relevance;
        std::cout   << " relevance: " << relevance << " abs_rel: " << absoluteRelevance  << std::endl;
        RelativeIndex currentDoc = {i, (float) relevance};
        result.push_back(currentDoc);
    }
    
    for (auto &doc: result)
        doc.rank /= (float) absoluteRelevance;

    std::sort(result.begin(), result.end(), [](const RelativeIndex &first, const RelativeIndex &second) {
        if (first.rank == second.rank)
            return first.doc_id < second.doc_id;
        else
            return first.rank > second.rank;
    });
   
   // if (result.size() > responsesLimit)
       // result.resize(responsesLimit);

    return result;
} 

void SearchServer::printResult(const std::vector<std::vector<RelativeIndex>> &answers) {
   // std::cout << std::endl;

    for (int i = 0; i < answers.size(); ++i) {
        //std::cout << "request" << WordHandler::getPositionNumber(i + 1) << ":" << std::endl;

        if (answers[i].empty())
            std::cout << "  result: false" << std::endl;
        else {
            std::cout << "  result: true" << std::endl;
            std::cout << "  relevance:" << std::endl;
        }

        for (auto j : answers[i])
            std::cout << "    docID: " << j.doc_id << ", rank: " << j.rank << std::endl;

        std::cout << std::endl;
    }
}


std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
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
        typedef std::pair<std::string,int> word_frec_pair;

        for (auto query : queries_input) {
           docs_ids.clear();
        
            std::vector<std::string> query_set = create_unique_query_words(query);
            int max_abs_relevance = 0;
            int abs_relevance = 0;

            for(auto it : query_set) {
               
                std::vector<word_frec_pair> pair_vec;
               
                std::vector<Entry> word_counts = _index.GetWordCount(it);  
               
                int frequency = 0;

                for(auto& e : word_counts) {
                   frequency += e.count;
                }
                pair_vec.push_back(std::make_pair(it,frequency));
                std::sort(pair_vec.begin(),pair_vec.end(),[](auto &left, auto &right) {
                   
                    return left.second < right.second;
                });
                //docs_ids.clear();

                for(auto& e : pair_vec) {
                    
                    std::vector<Entry> word_request_counts = _index.GetWordCount(e.first);
                   
                    for (const auto& entry : word_request_counts) {
                        docs_ids.push_back(entry.doc_id);                      
                    }

                    for(auto& it: docs_ids) {
                
                       for (const auto& entry : word_counts) {
                                                   
                           if(entry.doc_id == it) {
                               abs_relevance += entry.count;
                           }                           
                        }
                  
                    } 
                }
           
                std::cout << std::endl;
        
            }
            relative_indixes = calculate_relevance(docs_ids, query_set);
            result.push_back(relative_indixes);
            i++;
        }
       
        printResult(result);
        

        return result;
    }

