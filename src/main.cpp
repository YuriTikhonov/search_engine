#include "converter_json.h"


int main() {
    std::vector<std::string> text_contents;
    ConverterJSON converter;
 
    try {
        text_contents = converter.GetTextDocuments();
    }
    catch (const std::exception& e) {
        std::cout << "Caution! File is missing " << e.what();
        Sleep(int(5000)); //записать ошибку в лог
    }

    InvertedIndex inverted_index;    
    inverted_index.UpdateDocumentBase(text_contents);

    std::cout << "response_lim: " << converter.GetResponsesLimit() << std::endl;
    std::vector<std::string> vec = converter.GetRequests();

    for(auto it : vec) {// Здесь надо ограничить количество запросов < 1000
       // std::cout << "Request: " << it << std::endl; // количество слов в запросе от 1 до 10
    }

    return 0;
}
