#pragma once
#include "converter_json.h"

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <iterator>
#include <thread>
#include <mutex>
#include <algorithm>
#include <istream>


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
    std::mutex freqDictionaryAccess;
    std::istringstream docStream;    

    public:
    InvertedIndex() = default;

    void UpdateDocumentBase(std::vector<std::string> input_docs);

    std::vector<Entry> GetWordCount(const std::string& word);
   
    private:
    void fill_freq_dictionary(const std::string& in_word, size_t doc_num);
   
    void printIndex();


    void process_text_by_thread(const std::string& in_text, size_t& n);

    void optimize_threads_pool_with_hardware(const std::vector<std::string>& in_text_docs);
};
