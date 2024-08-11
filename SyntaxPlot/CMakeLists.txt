cmake_minimum_required(VERSION 3.4)
project(SyntaxPlot)

add_executable(${PROJECT_NAME}
main.cpp
syntaxplottersyntaxvisitor.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/Parser)
target_link_libraries(${PROJECT_NAME} PRIVATE Parser)