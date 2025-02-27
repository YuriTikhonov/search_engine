#include "InvertedIndex.h"


void InvertedIndex::fill_freq_dictionary(const std::string& in_word, size_t doc_num) {
    
    if(freq_dictionary.find(in_word) == freq_dictionary.end()) {
        std::vector<Entry> entry{{doc_num,1}};
        std::lock_guard<std::mutex> lock(freq_dictionary_access);
        freq_dictionary.emplace(std::make_pair(in_word, entry));
    }
    else {

        for(size_t i = 0; i < freq_dictionary.at(in_word).size(); ++i) {
            
            if(freq_dictionary.at(in_word)[i].doc_id == doc_num) {
                std::lock_guard<std::mutex> lock(freq_dictionary_access);
                freq_dictionary.at(in_word)[i].count++;
             
                break;
            }
            else if(i == freq_dictionary.at(in_word).size() - 1) {
                std::lock_guard<std::mutex> lock(freq_dictionary_access);
                freq_dictionary.at(in_word).push_back(Entry{doc_num,1});
                
                break;  
            }   
        }         
    }  
}


void InvertedIndex::process_text_by_thread(const std::string& in_text, size_t& n) {
    std::istringstream doc_stream(in_text);

    while (doc_stream) {
        std::string word;
        doc_stream >> word;
        
        if (word.empty()) {
            continue;
        }
        else {       
           fill_freq_dictionary(word, n-1);
        }
    }
}


void InvertedIndex::optimize_threads_pool_with_hardware(const std::vector<std::string>& in_text_docs) {
    size_t hard_ware_concurrency = std::thread::hardware_concurrency();
    size_t thread_numbers = (in_text_docs.size() > hard_ware_concurrency) ?  hard_ware_concurrency: in_text_docs.size();
    size_t  block_size = in_text_docs.size()/thread_numbers;
    size_t start = 0;
    std::vector<std::thread> threads_pool{};
   
    for(size_t i = 0; i < thread_numbers-1; ++i) {

        if(in_text_docs.size() > hard_ware_concurrency) {
            for(size_t j = start;j < (block_size + start);++j) {   
                std::this_thread::sleep_for(std::chrono::milliseconds(1));      
                threads_pool.emplace_back(&InvertedIndex::process_text_by_thread, this, std::ref(in_text_docs[j]), std::ref(j));
            }
            start += block_size;
        }
    }
  
    for(size_t j = start; j < in_text_docs.size(); ++j) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); 
        threads_pool.emplace_back(&InvertedIndex::process_text_by_thread, this, std::ref(in_text_docs[j]), std::ref(j));
    }

    for(auto& t: threads_pool) {
        if(t.joinable()) t.join();
    }
  } 


void InvertedIndex::UpdateDocumentBase( std::vector<std::string> in_text_docs) {
    optimize_threads_pool_with_hardware(in_text_docs);  
}
 

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
       
    return freq_dictionary[word];
}
