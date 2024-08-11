cmake_minimum_required(VERSION 3.4)
project(Parser)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

find_package(FLEX 2.6 REQUIRED)

set(PARSER_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(LEXER_OUT "${PARSER_DIR}/lexer.cpp")

# flex_target(LEXER "${SRC_DIR}/anna.l" "${LEXER_OUT}" DEFINES_FILE "${PARSER_DIR}/lexer.h")
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
