TARGET = Parser
TEMPLATE = lib
CONFIG -= qt
CONFIG += lex c++11

DEFINES += PARSER_LIBRARY

LEXSOURCES += anna.l

SOURCES += parser.cpp \
    lex_helper.cpp \
    lexertoken.cpp \
    annasyntax.cpp \
    annatoken.cpp \
    parser_global.cpp

HEADERS += parser.h\
        parser_global.h \
    lex_helper.h \
    lexertoken.h \
    annasyntax.h \
    annatoken.h

OTHER_FILES += anna.ebnf

unix {
    target.path = /usr/lib
    INSTALLS += target
}

lex.CONFIG += target_predeps
yacc_impl.CONFIG += target_predeps
yacc_decl.CONFIG += target_predeps
