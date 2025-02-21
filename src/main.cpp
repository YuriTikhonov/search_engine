#include "converter_json.h"
#include "SearchServer.h"
#include "InvertedIndex.h"

int main() {
    ConverterJSON converter; 
    std::cout << "Search Engine App started" << std::endl;
    InvertedIndex invertedIndex;
    SearchServer server(invertedIndex);
    std::vector<std::string> queries = converter.GetRequests(); 
    server.search(queries);
    
    return 0;
}
