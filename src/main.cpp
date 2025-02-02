#include "converter_json.h"
#include "SearchServer.h"
#include <thread>
#include <mutex>


int main() {
    ConverterJSON converter;
    InvertedIndex inverted_index;
     

    try {
        inverted_index.UpdateDocumentBase(converter.GetTextDocuments());
    }
    catch (const std::exception& e) {
        std::cout << "Caution! File is missing " << e.what();
        Sleep(int(5000)); //записать ошибку в лог
    }
    

    std::cout << "response_lim: " << converter.GetResponsesLimit() << std::endl;
    std::vector<std::string> queries = converter.GetRequests();

    for(auto it : queries) {// Здесь надо ограничить количество запросов < 1000
        std::cout << "Request: " << it << std::endl; // количество слов в запросе от 1 до 10
    }
     std::vector<std::vector<Entry>> result;

    for(auto& request : queries) { // это тестовый прогон, работает для одного слоав в запросе
        std::vector<Entry> word_count = inverted_index.GetWordCount(request);
        result.push_back(word_count);
    }

    for(int i = 0; i < result.size();++i) {
        std::cout << "Words count for request: " << queries[i] << " : ";
        for(auto& entry : result[i]) {
            std::cout << "(" << entry.doc_id << ", " << entry.count << ") ";
        }
        std::cout << std::endl;
    }    
    //inverted_index.printIndex();
     // SearchServer server(inverted_index);
     //SearchServer server;
    // std::vector<std::vector<RelativeIndex>> relative_indx_vec = server.search(queries);

    return 0;
}
