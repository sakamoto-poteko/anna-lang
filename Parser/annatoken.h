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
#include "parser.h"
#include "annasyntax.h"


class AnnaToken;
typedef std::shared_ptr<AnnaToken> gcToken;

class AnnaToken : public AnnaSyntaxNode
{
public:
    AnnaToken(Tokens token, gcString text, int row, int col,
              int width, std::vector<std::string> trailingComments = std::vector<std::string>())
        : _token(token), _text(text), _row(row),
          _col(col), _width(width), _trailing_comments(trailingComments)
    {}

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

    gcString identifier() { return _identifier; }

protected:
    gcString _identifier;
};

class RealToken : public AnnaToken
{
public:
    RealToken(Tokens token, gcString text, int row, int col, int width, double real,
                    std::vector<std::string> trailingComments = std::vector<std::string>())
        : AnnaToken(token, text, row, col, width, trailingComments), _real(real)
    {}

    double real() { return _real; }

protected:
    double _real;
};

class IntegerToken : public AnnaToken
{
public:
    IntegerToken(Tokens token, gcString text, int row, int col, int width, int integer,
                 std::vector<std::string> trailingComments = std::vector<std::string>())
        : AnnaToken(token, text, row, col, width, trailingComments), _integer(integer)
    {}

    int integer() { return _integer; }

protected:
    int _integer;
};

class BooleanToken : public AnnaToken
{
public:
    BooleanToken(Tokens token, gcString text, int row, int col, int width, int boolean,
                 std::vector<std::string> trailingComments = std::vector<std::string>())
        : AnnaToken(token, text, row, col, width, trailingComments), _boolean(boolean)
    {}

    int boolean() { return _boolean; }

protected:
    int _boolean;
};

class StringToken : public AnnaToken
{
public:
    StringToken(Tokens token, gcString text, int row, int col, int width, gcString identifier,
                    std::vector<std::string> trailingComments = std::vector<std::string>())
        : AnnaToken(token, text, row, col, width, trailingComments), _string(identifier)
    {}

    gcString string() { return _string; }

protected:
    gcString _string;
};


#endif // ANNATOKEN_H
