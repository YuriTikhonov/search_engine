cmake_minimum_required(VERSION 3.22)

set(PROJECT search_engine)
project(${PROJECT} VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
add_subdirectory(src )
add_subdirectory(nlohmann_json)
add_subdirectory(unitTests)
target_link_libraries(${PROJECT} PRIVATE nlohmann_json::nlohmann_json)
