#include "converter_json.h"
#include "SearchServer.h"
#include "InvertedIndex.h"


int main() {

    try {
        ConverterJSON converter;
        std::cout << "Search engine started" << std::endl; 
        InvertedIndex invertedIndex;
        SearchServer server(invertedIndex);
        std::vector<std::string> queries = converter.GetRequests(); 
        server.search(queries);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what();
    }
        
    return 0;
}
