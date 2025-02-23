cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME unit_tests)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(${PROJECT_NAME}
       mainTest.cpp ../include/InvertedIndex.h ../src/InvertedIndex.cpp ../include/converter_json.h ../include/SearchServer.h ../src/converter_json.cpp ../src/SearchServer.cpp
        )

#target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_include_directories(${PROJECT_NAME} PUBLIC ${MY_INCLUDE_DIR})


target_link_libraries(${PROJECT_NAME} PRIVATE gtest gtest_main nlohmann_json::nlohmann_json)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
