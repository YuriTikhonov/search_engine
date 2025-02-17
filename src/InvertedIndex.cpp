#include "../include/InvertedIndex.h"

void InvertedIndex::fill_freq_dictionary(const std::string& in_word, size_t doc_num) {
 
    if(freq_dictionary.find(in_word) == freq_dictionary.end()) {
        std::vector<Entry> entry{{doc_num,1}};
        std::lock_guard<std::mutex> lock(freqDictionaryAccess);
        freq_dictionary.emplace(std::make_pair(in_word, entry));
    }
    else {

        for(size_t i = 0; i < freq_dictionary.at(in_word).size(); ++i) {
            
             if(freq_dictionary.at(in_word)[i].doc_id == doc_num) {
                std::lock_guard<std::mutex> lock(freqDictionaryAccess);
                freq_dictionary.at(in_word)[i].count++;
             
                break;
            }
            else if(i == freq_dictionary.at(in_word).size() - 1) {
                std::lock_guard<std::mutex> lock(freqDictionaryAccess);
                freq_dictionary.at(in_word).push_back(Entry{doc_num,1});
                
                break;  
            }   
        }         
    }  
}


void InvertedIndex::process_text_by_thread(const std::string& in_text, size_t& n) {
    std::istringstream docStream(in_text);

    while (docStream) {
        std::string word;
        docStream >> word;
        
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
   
    for(size_t i = 0; i < thread_numbers - 1; ++i) {

        if(in_text_docs.size() > hard_ware_concurrency) {
            for(size_t j = start;j < (block_size + start);++j) {  
                Sleep(int(1));        
                threads_pool.push_back(std::move(std::thread(process_text_by_thread,this,
                 std::ref(in_text_docs[j]), std::ref(j))));
            }
            start += block_size;
        }
    }
 
    for(size_t j = 0; j < in_text_docs.size(); ++j) {
        Sleep(int(1));
        threads_pool.push_back(std::move(std::thread(process_text_by_thread, this,
         std::ref(in_text_docs[j]), std::ref(j))));
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

void InvertedIndex::printIndex() {
       std::cout << "---------begin-----" << std::endl;

		for (auto ind:freq_dictionary) {
		    std::cout << ind.first << ": ";

		    for (auto vec:ind.second){
			    std::cout << "{" <<vec.doc_id<< "," << vec.count <<"}";
		    }
           std::cout << std::endl; 
	    }
        std::cout << "---------end----" << std::endl;
    }
