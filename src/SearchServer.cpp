#include "SearchServer.h"

 SearchServer::SearchServer( InvertedIndex idx):  _index(idx) {};

std::vector<std::string> SearchServer::create_unique_query_words(std::string& words) {
    std::unordered_set<std::string>query_set;
    std::vector<std::string> query_vec;
    int queries_count = 0;
        
      //number of requests < 1000
      if(queries_count < 1000) {
        std::istringstream query_stream(words);        
        int query_word_count = 0;

        while(query_stream) {
          std::string word;
          query_stream >> word;
            
          // the number of words in a query is from 1 to 10
          if(word != "" && query_word_count < 10) {
            query_set.insert(word);
            query_word_count++;
          }  
        }
      }
    std::unordered_set<std::string> :: iterator itr;

    for (itr = query_set.begin(); itr != query_set.end(); itr++) {
      query_vec.push_back(*itr);
    }

    return query_vec;
} 
std::vector<RelativeIndex>  SearchServer::rate_relevance(const std::vector<size_t> &docs_ids_vec,
  const std::vector<std::string> &request) 
{
  std::vector<RelativeIndex> result;
  
  size_t max_abs_relevance = 0;
   
  for (auto &i:docs_ids_vec) {
    size_t relevance = 0.0f;
    size_t absolute_relevance = 0;  
   
    for (auto &word: request) {
      size_t frequency = 0;
      std::vector<Entry>temp = _index.GetWordCount(word);
          
      for(auto& w : temp) {
                
        if (w.doc_id == i) {
         frequency += w.count;
        }       
      }
      absolute_relevance += frequency;
    }
   
   if(absolute_relevance > max_abs_relevance) {
    max_abs_relevance = absolute_relevance;
   }
       
    RelativeIndex current_doc = {i, (float) absolute_relevance};
      result.push_back(current_doc);
  }

  for (auto &doc: result) {
    doc.rank /= (float) max_abs_relevance;
  }

  std::sort(result.begin(), result.end(), [](const RelativeIndex &first, const RelativeIndex &second) {

    if (first.rank == second.rank) {

      return first.doc_id < second.doc_id;
    }        
    else {

      return first.rank > second.rank;
    }        
  });

  int responses_limit = _converter.GetResponsesLimit();
   
  if (result.size() > responses_limit) {
    result.resize(responses_limit);
  }
        
  return result;
} 

void SearchServer::print_result(const std::vector<std::vector<RelativeIndex>> &answers) {
  std::cout << "{" << std::endl << "answers:" << std::endl;

  for (int i = 0; i < answers.size(); ++i) {
    std::cout << "   request00" << i+1 << ":" << "{ " << std::endl;

    if (answers[i].empty()) {
      std::cout << "         result: false" << std::endl;
    }  
    else {
      std::cout << "         result: true," << std::endl;
      std::cout << "         relevance:{" << std::endl;
    }

    for (auto j : answers[i]) {
      std::cout << "             doc_id: " << j.doc_id << ", rank: " << j.rank << std::endl;
    } 
    if (!answers[i].empty()) { 
    std::cout <<"        }" << std::endl;
    std::cout <<"  }" << std::endl;
    }
  }
  std::cout <<"  }" << std::endl;
  std::cout <<"}" << std::endl;
}


std::vector<std::vector<std::pair<int, float>>> SearchServer::prepair_json_format(const std::vector<std::vector<RelativeIndex>>& _indx)
{
  std::vector<std::vector<std::pair<int, float>>> json_set;
  
  for (int i = 0; i < _indx.size(); ++i) {
    std::vector<std::pair<int, float>> idx;
    if (_indx[i].empty()) {
     
      for(auto j : _indx[i]) {
        idx.push_back(std::make_pair(-1, 0.0f));
      }
    }
    else {
    
      for (auto j : _indx[i]) {
        idx.push_back(std::make_pair(j.doc_id, j.rank));
      }
    }    
      json_set.push_back(idx);
  }
    
  return json_set;
}
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
  std::vector<std::vector<RelativeIndex>> result;
  std::vector<RelativeIndex> relative_indixes;

  _index.UpdateDocumentBase(_converter.GetTextDocuments());           
  typedef std::pair<std::string,int> word_frec_pair;

  for (auto query : queries_input) {  
    std::vector<std::string> query_set = create_unique_query_words(query);
    int max_abs_relevance = 0;
    int abs_relevance = 0;
    std::set<size_t> docs_id_set;
      
    for(auto it : query_set) {     
      std::vector<word_frec_pair> pair_vec;     
      std::vector<Entry> word_counts = _index.GetWordCount(it);          
      int frequency = 0;

      for(auto& e : word_counts) {
        frequency += e.count;
      }
      pair_vec.push_back(std::make_pair(it,frequency));
      std::sort(pair_vec.begin(),pair_vec.end(),[](auto &left, auto &right) {
                   
        return left.second < right.second;
      });
      std::vector<Entry> word_request_counts;
      
      for(auto& e : pair_vec) {       
       word_request_counts = _index.GetWordCount(e.first);

        for (const auto& entry : word_request_counts) {
          docs_id_set.insert(entry.doc_id);                      
        }
      }     
    }
    docs_ids.assign(docs_id_set.begin(), docs_id_set.end());
    relative_indixes = rate_relevance(docs_ids, query_set);
    result.push_back(relative_indixes);
  }    
  print_result(result);
  std::vector<std::vector<std::pair<int, float>>> json_answer = prepair_json_format(result);
  _converter.putAnswers(json_answer);

  return result;
}
