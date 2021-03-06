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


#ifndef ANNATOKEN_H
#define ANNATOKEN_H

#include "parser_global.h"
#include "annanode.h"
#include "annasyntaxvisitor.h"


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

class AnnaToken : public AnnaNode
{
public:
    AnnaToken(Tokens token, gcString text, int row, int col,
              int width, std::vector<std::string> trailingComments = std::vector<std::string>())
        : _token(token), _text(text), _row(row),
          _col(col), _width(width), _trailing_comments(trailingComments)
    {}

    virtual void Accept(AnnaSyntaxVisitor &visitor);

    Tokens token() { return _token; }
    gcString text() { return _text; }
    int row() { return _row; }
    int col() { return _col; }
    int width() { return _width; }
    std::vector<std::string> trailingComments() { return _trailing_comments; }

protected:
    Tokens _token;

    gcString _text;
    int _row;
    int _col;
    int _width;
    std::vector<std::string> _trailing_comments;
};

class IdentifierToken : public AnnaToken
{
public:
    IdentifierToken(Tokens token, gcString text, int row, int col, int width, gcString identifier,
                    std::vector<std::string> trailingComments = std::vector<std::string>())
        : AnnaToken(token, text, row, col, width, trailingComments), _identifier(identifier)
    {}

    virtual void Accept(AnnaSyntaxVisitor &visitor);


    gcString identifier() { return _identifier; }

protected:
    gcString _identifier;
};

class LiteralToken : public AnnaToken
{
public:
    enum LiteralType {
        Real,
        Integer,
        Boolean,
        String
    };

    virtual void Accept(AnnaSyntaxVisitor &visitor);

    LiteralType literalType();

protected:
    LiteralToken(Tokens token, gcString text, int row, int col, int width,
                 std::vector<std::string> trailingComments = std::vector<std::string>())
        : AnnaToken(token, text, row, col, width, trailingComments)
    {}

    LiteralType _literalType;
};

class RealToken : public LiteralToken
{
public:
    RealToken(Tokens token, gcString text, int row, int col, int width, double real,
                    std::vector<std::string> trailingComments = std::vector<std::string>())
        : LiteralToken(token, text, row, col, width, trailingComments), _real(real)
    {
         _literalType = Real;
    }

    virtual void Accept(AnnaSyntaxVisitor &visitor);

    double real() { return _real; }

protected:
    double _real;
};

class IntegerToken : public LiteralToken
{
public:
    IntegerToken(Tokens token, gcString text, int row, int col, int width, int integer,
                 std::vector<std::string> trailingComments = std::vector<std::string>())
        : LiteralToken(token, text, row, col, width, trailingComments), _integer(integer)
    {
        _literalType = Integer;
    }

    virtual void Accept(AnnaSyntaxVisitor &visitor);

    int integer() { return _integer; }

protected:
    int _integer;
};

class BooleanToken : public LiteralToken
{
public:
    BooleanToken(Tokens token, gcString text, int row, int col, int width, int boolean,
                 std::vector<std::string> trailingComments = std::vector<std::string>())
        : LiteralToken(token, text, row, col, width, trailingComments), _boolean(boolean)
    {
        _literalType = Boolean;
    }

    virtual void Accept(AnnaSyntaxVisitor &visitor);

    int boolean() { return _boolean; }

protected:
    int _boolean;
};

class StringToken : public LiteralToken
{
public:
    StringToken(Tokens token, gcString text, int row, int col, int width, gcString identifier,
                    std::vector<std::string> trailingComments = std::vector<std::string>())
        : LiteralToken(token, text, row, col, width, trailingComments), _string(identifier)
    {
         _literalType = String;
    }

    virtual void Accept(AnnaSyntaxVisitor &visitor);

    gcString string() { return _string; }

protected:
    gcString _string;
};


#endif // ANNATOKEN_H
