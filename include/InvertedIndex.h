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
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
    std::mutex freqDictionaryAccess;
   
    void show_docs() {

         for(int i = 0;i < docs.size(); ++i) {
            std::cout << i << " doc_id: " <<  docs[i] << std::endl;
        }
    }    

    public:
   // std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь

    InvertedIndex() = default;

    void UpdateDocumentBase(std::vector<std::string> input_docs);

    std::vector<Entry> GetWordCount(const std::string& word);

    void fill_freq_dictionary(std::string& in_word, size_t doc_num);

    void printIndex();

    void process_text_by_thread(const std::string& in_text, int n);
};

//void process_text_by_thread(const std::string& in_text, int n);//эти две функции надо сделать с возвращением значений, чтобы работать с одним экземпляром индекса
void optimize_threads_pool_with_hardware(const std::vector<std::string>& inTextDocs);
