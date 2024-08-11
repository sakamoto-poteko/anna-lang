cmake_minimum_required(VERSION 3.4)
project(Anna)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

find_package(FLEX 2.6 REQUIRED)

add_subdirectory(Parser)
add_subdirectory(Symbol)
add_subdirectory(ParserTest)
add_subdirectory(SyntaxPlot)