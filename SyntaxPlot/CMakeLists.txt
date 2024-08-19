cmake_minimum_required(VERSION 3.5)
project(SyntaxPlot)

find_package(Boost 1.45.0 COMPONENTS graph)

add_executable(${PROJECT_NAME}
main.cpp
syntaxplottersyntaxvisitor.cpp
)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/Parser)
target_link_libraries(${PROJECT_NAME} PRIVATE Parser)