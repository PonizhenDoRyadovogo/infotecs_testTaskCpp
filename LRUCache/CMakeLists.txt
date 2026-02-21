cmake_minimum_required(VERSION 3.10)

project(LRUCache VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(lru_cache_test main.cpp)

target_link_libraries(lru_cache_test PRIVATE Threads::Threads)

target_include_directories(lru_cache_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})