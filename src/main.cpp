#include "converter_json.h"
#include "SearchServer.h"
#include "InvertedIndex.h"

int main() {
    ConverterJSON converter;
    InvertedIndex* inverted_index = new InvertedIndex(*inverted_index);
    SearchServer server(*inverted_index);
    std::vector<std::string> queries = converter.GetRequests(); 
    server.search(queries);
    delete inverted_index;
    
    return 0;
}
