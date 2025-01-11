#include "InvertedIndex.h"


void InvertedIndex::textOperation(const std::string &inText, size_t n) {
    std::string clearWord;
    std::istringstream docStream(inText);
    while (docStream) {
        std::string tempWord;
        docStream >> tempWord;
        clearWord = tempWord;//invertedIndex->prepareWord(tempWord);
        freqDictionaryAccess.lock();
        std::vector<Entry> tempEntry = InvertedIndex::GetWordCount(clearWord);
        freqDictionaryAccess.unlock();
        bool wordExist = false;
        bool entryExist = false;

        if (!tempEntry.empty()) {
            wordExist = true;
            freqDictionaryAccess.lock();
            for(auto it : tempEntry)
            {
                if(it.doc_id == n)
                {
                    entryExist = true;
                }
            }
            freqDictionaryAccess.unlock();
        }
        if(!entryExist)
        {
            freqDictionaryAccess.lock();
            tempEntry.push_back(Entry{n, 1});
            freqDictionaryAccess.unlock();
        }
        freqDictionaryAccess.lock();
        //invertedIndex->fillFreqDict(tempEntry, wordExist, clearWord);
        freqDictionaryAccess.unlock();
    }
}


void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& inTextDocs) {
    docs = inTextDocs;
    Entry entry; 

    unsigned int hardWareConcurrency = std::thread::hardware_concurrency();
    unsigned int threadNumbers = (inTextDocs.size() > hardWareConcurrency) ?  hardWareConcurrency: inTextDocs.size();
    unsigned int  blockSize = inTextDocs.size()/threadNumbers;
   std::cout << "tread numbers: " << threadNumbers << ", number of docs is indexed: " << inTextDocs.size() <<
   ", block size: " << blockSize << std::endl;

    size_t start = 0;

    std::thread* threads = new std::thread[threadNumbers];
/*нужно полностью пересмотреть обработку текста: поиск по индексу неверен!! 
    функцию textOperation переосмыстить!!!

    for(unsigned int i = 0;i < threadNumbers - 1;++i)
    {
        for(unsigned int j = start;j < (blockSize + start);++j)
        {
            if(!threads[i].joinable())
            {
                threads[i] = std::thread(textOperation, inTextDocs[j],j);
            }
            threads[i].join();
        }
        start += blockSize;
    }

    for(unsigned int j = start;j < inTextDocs.size();++j)
    {
        if(!threads[threadNumbers - 1].joinable())
            threads[threadNumbers - 1] = std::thread(textOperation, inTextDocs[j],j);
        threads[threadNumbers - 1].join();
    }
*/    
/*   
   // std::mutex entry_access;

    for(int i = 0; i < docs.size();i++) {
       // entry_access.lock();
        //здесь? нужно запустить нитку потока?
        entry.doc_id = i;
        std::cout << "Loading " << entry.doc_id << ": " << docs[i] << std::endl;
        //entry_access.unlock();
    }
*/     
}


std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    std::vector<Entry> result;
   // freqDictionaryAccess.lock();
    auto it = freq_dictionary.find(word);

    if(it!= freq_dictionary.end()) {
        result = it->second; // ????? здесь должен быть подсчет слов
    }
   // freqDictionaryAccess.unlock();

    return result;
}
