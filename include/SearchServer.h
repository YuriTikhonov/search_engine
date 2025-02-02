#pragma once

#include "InvertedIndex.h"
#include <vector>
#include <string>

struct RelativeIndex {
    size_t doc_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const {

        return (doc_id == other.doc_id && rank == other.rank);
    }
};


class SearchServer {
    //InvertedIndex _index;
    public:
    
    /*
    * @param idx в конструктор класса передаётся ссылка на класс
    InvertedIndex,
    * чтобы SearchServer мог узнать частоту слов встречаемых в
    запросе
    */
    //SearchServer() = default;
    //SearchServer(const InvertedIndex& idx): _index(idx){ };
    /**
    * Метод обработки поисковых запросов
    * @param queries_input поисковые запросы взятые из файла
    requests.json
    * @return возвращает отсортированный список релевантных ответов для
    заданных запросов
    */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
};
