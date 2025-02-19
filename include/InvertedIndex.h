#pragma once
#include "converter_json.h"

#include <map>
#include <iterator>
#include <thread>
#include <mutex>
#include <algorithm>

struct Entry {
    size_t doc_id, count;
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
        count == other.count);
    }
};


class InvertedIndex {
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;
    std::mutex freq_dictionary_access;
    std::istringstream doc_stream;    

    public:
    InvertedIndex() = default;
    InvertedIndex(InvertedIndex& indx_) {};
  

    void UpdateDocumentBase(std::vector<std::string> input_docs);

    std::vector<Entry> GetWordCount(const std::string& word);
    void print_index();
    private:
    void fill_freq_dictionary(const std::string& in_word, size_t doc_num);
   
    


    void process_text_by_thread(const std::string& in_text, size_t& n);

    void optimize_threads_pool_with_hardware(const std::vector<std::string>& in_text_docs);
};
