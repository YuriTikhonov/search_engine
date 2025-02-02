cmake_minimum_required(VERSION 3.22)

add_executable(${PROJECT}  main.cpp converter_json.cpp InvertedIndex.cpp SearchServer.cpp
 ${MY_INCLUDE_DIR}/converter_json.h ${MY_INCLUDE_DIR}/InvertedIndex.h ${MY_INCLUDE_DIR}/SearchServer.h)
target_include_directories(${PROJECT}  PUBLIC ${MY_INCLUDE_DIR})
