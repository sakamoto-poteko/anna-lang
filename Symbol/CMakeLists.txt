cmake_minimum_required(VERSION 3.5)
project(Symbol)

add_library(${PROJECT_NAME}
symbol.cpp
exportedsymbolvisitor.cpp
compilationunitsymbolcollection.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Parser)
target_link_libraries(${PROJECT_NAME} PRIVATE Parser)