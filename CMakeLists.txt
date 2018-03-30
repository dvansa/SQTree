cmake_minimum_required (VERSION 2.8.11)
project (SQTree)

add_executable (sqtree ${CMAKE_CURRENT_SOURCE_DIR}/src/sqtree.cpp)

target_include_directories (sqtree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)