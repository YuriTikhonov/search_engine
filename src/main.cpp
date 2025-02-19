#include "converter_json.h"
#include "SearchServer.h"
#include "InvertedIndex.h"

int main() {
    ConverterJSON converter;
    InvertedIndex* invertedIndex = new InvertedIndex(*invertedIndex);
    SearchServer server(*invertedIndex);
    std::vector<std::string> queries = converter.GetRequests(); 
    server.search(queries);
    delete invertedIndex;
    
    return 0;
}
