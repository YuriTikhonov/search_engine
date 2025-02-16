#include "converter_json.h"
#include "SearchServer.h"
#include "InvertedIndex.h"
#include <thread>
#include <mutex>
#include <unordered_set>


int main() {
    ConverterJSON converter;
    InvertedIndex* inverted_index = new InvertedIndex(*inverted_index);
    SearchServer server(*inverted_index);
  
    std::cout << "response_lim: " << converter.GetResponsesLimit() << std::endl;
    std::vector<std::string> queries = converter.GetRequests();
    
     std::vector<std::vector<RelativeIndex>> relative_indx_vec = server.search(queries);
     
    delete inverted_index;
    
    return 0;
}
