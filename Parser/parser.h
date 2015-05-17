/**************************************************************************
 * Copyright (c) 2015 Afa.L Cheng <afa@afa.moe>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/


#ifndef PARSER_H
#define PARSER_H

#include "parser_global.h"

enum Tokens
{
    END = 0,
    DEF = 257,
    MAIN,
    IF,
    ELSE,
    WHILE,
    GE,
    LE,
    EE,
    NE,
    AND,
    OR,
    XOR,
    GT,
    LT,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    NOT,
    BITNOT,
    ANDAND,
    OROR,
    EQ,
    IMPORT,
    RETURN,
    VAR,
    USER_FUNCTION_IDENTIFIER,
    IDENTIFIER,
    VARIABLE_IDENTIFIER,
    STRING,
    REAL,
    INTEGER,
    BOOLEAN,
    T,
    OPEN_PAREN,
    CLOSE_PAREN,
    OPEN_BRACE,
    CLOSE_BRACE,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    COMMA,

    ERROR = -1
};

class AnnaParser
{
public:
    AnnaParser();

    // FIXME: Change reutrn type
    //SyntaxTree ParseText(gcString str);

protected:
    //CompilationUnitSyntax ParseCompilationUnit();

};



#endif // PARSER_H
