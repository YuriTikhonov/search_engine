#include "converter_json.h"
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
    std::vector<std::string> vec = converter.GetRequests();

    for(auto it : vec) {// Здесь надо ограничить количество запросов < 1000
       // std::cout << "Request: " << it << std::endl; // количество слов в запросе от 1 до 10
    }
    
    //inverted_index.printIndex(); // надо ждать, когда нитки отработают.

    return 0;
}
