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

    void textOperation(const std::string &inText, size_t n); // необходимость функции под вопросом

    public:

    InvertedIndex() = default;

    void UpdateDocumentBase(const std::vector<std::string>& input_docs);

    std::vector<Entry> GetWordCount(const std::string& word);
};
