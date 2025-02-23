#include "gtest/gtest.h"

#include "InvertedIndex.h"
#include "SearchServer.h"

TEST(sample_test_case, sample_test)
{
  EXPECT_EQ(1, 1);
}
using namespace std;


void TestInvertedIndexFunctionality(
  const vector<string>& test_docs,
  const vector<string>& test_requests,
  const std::vector<vector<Entry>>& expected
) {
  std::vector<std::vector<Entry>> test_result;
  InvertedIndex test_idx;
  test_idx.UpdateDocumentBase(test_docs);
  
  for(auto& request : test_requests) {
    std::vector<Entry> word_count = test_idx.GetWordCount(request);
    test_result.push_back(word_count);
  }
  ASSERT_EQ(test_result, expected);
}
TEST(TestCaseInvertedIndex, TestBasic) {
  const vector<string> docs_t = {
    "london is the capital of great britain",
    "big ben is the nickname for the Great bell of the striking clock"
  };
  const vector<string> test_requests = {"london", "the"};
  const vector<vector<Entry>> expected = {
    {{0, 1}},
    {{0, 1}, {1, 3}}
  };
  TestInvertedIndexFunctionality(docs_t,test_requests, expected);
}

TEST(TestCaseInvertedIndex, TestBasic2) {
  const vector<string> docs = {
    "milk milk milk milk water water water",
    "milk water water",
    "milk milk milk milk milk water water water water water",
    "americano cappuccino"
  };
  const vector<string> requests = {"milk", "water", "cappuccino"};
  const vector<vector<Entry>> expected = {
    {{0, 4}, {1, 1}, {2, 5}}, 
    {{0, 3}, {1, 2}, {2, 5}},
    {{3, 1}}
  };
  TestInvertedIndexFunctionality(docs, requests, expected);
  }


  TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const vector<string> docs_m = {
      "a b c d e f g h i j k l",
      "statement"
    };
    const vector<string> requests = {"m", "statement"};
    const vector<vector<Entry>> expected = {
      {},
      {{1, 1}}
    };
    TestInvertedIndexFunctionality(docs_m, requests, expected);
  }

TEST(TestCaseSearchServer, TestTop5) {
  const vector<string> docs_5 = {
  "london is the capital of great britain",
  "paris is the capital of france",
  "berlin is the capital of germany",
  "rome is the capital of italy",
  "madrid is the capital of spain",
  "lisboa is the capital of portugal",
  "bern is the capital of switzerland",
  "moscow is the capital of russia",
  "kiev is the capital of ukraine",
  "minsk is the capital of belarus",
  "astana is the capital of kazakhstan",
  "beijing is the capital of china",
  "tokyo is the capital of japan",
  "bangkok is the capital of thailand",
  "welcome to moscow the capital of russia the third rome",
  "amsterdam is the capital of netherlands",
  "helsinki is the capital of finland",
  "oslo is the capital of norway",
  "stockholm is the capital of sweden",
  "riga is the capital of latvia",
  "tallinn is the capital of estonia",
  "warsaw is the capital of poland"
};
const vector<string> test_requests = {"moscow is the capital of russia", "abracadabra"};
const std::vector<vector<RelativeIndex>> expected = {
  {
    {7, 1},
    {14, 1},
    {0, 0.666666687},
    {1, 0.666666687},
    {2, 0.666666687}
  },
  {

  }
};
InvertedIndex test_idx;

test_idx.UpdateDocumentBase(docs_5);
Sleep(int(10));
SearchServer srv(test_idx);
std::vector<vector<RelativeIndex>> test_result = srv.search(test_requests);
ASSERT_EQ(test_result, expected);
}
