
Search engine
=============

The Prototype of the corporate portal search system
---------------------------------------------------

A search engine is a console application  
(an executable file that runs on any server or computer)  
that performs a search and can be configured via JSON files

Features
--------

*   Before launching the application, the configuration file specifies the names  
    of the files that the engine will use to search.
*   The search engine crawls all the files and  
    indexes them so that it can then find the most  
    relevant documents for any search query.
*   The user sets a request via the requests.json file. A query is  
    a set of words that you need to use to find documents.
*   The search results are ranked, sorted and given to the user  
    . The maximum number of possible documents in the response is set in  
    the configuration file.
*   At the end, the program generates the answers.json file that records  
    the search results.

Tech
----

Search engine uses a number of open source projects to work properly:

*   To read/generate JSON documents, a third-party library [[https://github.com/nlohmann/json](https://github.com/nlohmann/json)] is used.
*   For unit tests, the GoogleTest[[https://github.com/google/googletest](https://github.com/google/googletest)] library is used.

Search engine itself is open source with a public repository \[[https://github.com/YuriTikhonov/search\_engine](https://github.com/YuriTikhonov/search_engine)]  
on GitHub.

Installation
------------

To be installed search engine requires:  
-CMake build system  
-MinGw-64 compiler

For the program to work correctly, it is necessary that the executable file be in the same directory as the files: config.json, requests.json.

Launch
------

Launching the program use the Windows command line:  
.\search_engine.exe  
Launching the tests use the Windows command line:  
.\unit_tests.exe

License
-------

MIT

**Free Software, Hell Yeah!**