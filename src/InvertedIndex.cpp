#include "InvertedIndex.h"

void InvertedIndex::fill_freq_dictionary(std::string& in_word, size_t doc_num) {
    
    if(freq_dictionary.find(in_word) == freq_dictionary.end()) {
        std::vector<Entry> entry{{doc_num,1}};
        freqDictionaryAccess.lock();
        freq_dictionary.emplace(std::make_pair(in_word, entry));
        freqDictionaryAccess.unlock();
    }
    
    else {

        for(size_t i = 0; i < freq_dictionary[in_word].size(); ++i) {
            if(freq_dictionary[in_word][i].doc_id == doc_num) {
                freqDictionaryAccess.lock();
                freq_dictionary[in_word][i].count++;
                freqDictionaryAccess.unlock();
                return;
            }
            
            else if(i == freq_dictionary[in_word].size() - 1) {
                freqDictionaryAccess.lock();
                freq_dictionary[in_word].push_back(Entry{doc_num,1});
                freqDictionaryAccess.unlock();
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
           fill_freq_dictionary(word, n);
        }
    }
}


void InvertedIndex::optimize_threads_pool_with_hardware(const std::vector<std::string>& inTextDocs) {
    size_t hardWareConcurrency = std::thread::hardware_concurrency();
    size_t threadNumbers = (inTextDocs.size() > hardWareConcurrency) ?  hardWareConcurrency: inTextDocs.size();
    size_t  blockSize = inTextDocs.size()/threadNumbers;
    std::cout << "tread numbers: " << threadNumbers << ", number of docs is indexed: " << inTextDocs.size() <<
       ", block size: " << blockSize << " yader: " << hardWareConcurrency << std::endl;

    size_t start = 0;
    std::vector<std::thread> threads_pool;
    
    for(size_t i = 0; i < threadNumbers - 1; ++i) {

        for(size_t j = start;j < (blockSize + start);++j) {           
            threads_pool.push_back(std::move(std::thread(process_text_by_thread,this, std::ref(inTextDocs[j]), std::ref(j))));
            // дополнительно к явным параметрам функции передаем указатель на экземпляр класса ( иначе не будет работать)
        }
        start += blockSize;
    }

    for(size_t j = start; j < inTextDocs.size(); ++j) {
        threads_pool.push_back(std::move(std::thread(process_text_by_thread, this, std::ref(inTextDocs[j]), std::ref(j))));
        // дополнительно к явным параметрам функции передаем указатель на экземпляр класса ( иначе не будет работать)
    }

    for(auto& t: threads_pool) {
        if(t.joinable()) t.join();
    }
    printIndex();
  } 

void InvertedIndex::UpdateDocumentBase( std::vector<std::string> inTextDocs) {
    docs = inTextDocs;
    optimize_threads_pool_with_hardware(inTextDocs);
   
}
   

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    std::vector<Entry> result;
    result = freq_dictionary[word];
    
    return result;
}

void InvertedIndex::printIndex() {
       freqDictionaryAccess.lock();
       std::cout << "---------begin-----" << std::endl;
		for (auto ind:freq_dictionary) {
            
		    std::cout << ind.first << ": ";

		    for (auto vec:ind.second){
			    std::cout << "{" <<vec.doc_id<< "," << vec.count <<"}";
		    }
            
           std::cout << std::endl;
           
	    }
        std::cout << "---------end----" << std::endl;
       freqDictionaryAccess.unlock(); 
    }
