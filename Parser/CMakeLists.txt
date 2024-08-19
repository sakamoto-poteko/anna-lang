cmake_minimum_required(VERSION 3.5)
project(Parser)

set(PARSER_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(LEXER_OUT "${PARSER_DIR}/lexer.cpp")

flex_target(LEXER "anna.l" "${LEXER_OUT}")

add_library(${PROJECT_NAME}
parser.cpp
lex_helper.cpp
lexertoken.cpp
annasyntax.cpp
annatoken.cpp
parser_global.cpp
annasyntaxvisitor.cpp
annanode.cpp
annasyntax_visitor.cpp
${LEXER_OUT}
)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
