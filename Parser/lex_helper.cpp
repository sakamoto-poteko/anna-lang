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

#include <cstring>
#include <cstdio>

#include "lexertoken.h"
#include "lex_helper.h"
#include "parser.h"

extern FILE* annain;
int annalex();
static bool use_temp_file_stream;
static std::string __lex_filename;

static void split_lines(FILE* in)
{
    std::vector<std::string>& source_rows = get_lex_source_rows();

    char buf[65536];
    long current_pos = ftell(in);
    char* line;
    do {
        line = fgets(buf, 65536, in);
        source_rows.push_back(std::string(buf));
    } while (line);
    fseek(in, current_pos, SEEK_SET);
}

bool lexer_init(FILE* in, const std::string &filename)
{
    lexer_finalize();

    use_temp_file_stream = false;
    annain = in;
    split_lines(in);

    __lex_filename = filename;
    return true;
}

bool lexer_init(char* in, size_t len, const std::string &filename)
{
    lexer_finalize();

    use_temp_file_stream = true;
    annain = tmpfile();
    fwrite(in, 1, len, annain);
    fseek(annain, 0, SEEK_SET);
    split_lines(annain);

    __lex_filename = filename;
    return true;
}

gcnToken tokenize()
{
    int lex_val = annalex();

    auto tComments = lexerToken.trailing_comments;
    lexerToken.trailing_comments.clear();

    switch (lex_val) {
        case DEF:
        case MAIN:
        case IF:
        case ELSE:
        case WHILE:
        case GE:
        case LE:
        case EE:
        case NE:
        case AND:
        case OR:
        case XOR:
        case GT:
        case LT:
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case MOD:
        case NOT:
        case BITNOT:
        case ANDAND:
        case OROR:
        case EQ:
        case IMPORT:
        case RETURN:
        case VAR:
        case OPEN_PAREN:
        case CLOSE_PAREN:
        case OPEN_BRACE:
        case CLOSE_BRACE:
        case OPEN_BRACKET:
        case CLOSE_BRACKET:
        case COMMA:
        case T:
            return std::make_shared<AnnaToken>(static_cast<Tokens>(lex_val),
                                               lexerToken.text,
                                               lexerToken.token_row,
                                               lexerToken.token_col,
                                               lexerToken.token_leng,
                                               tComments);

        case USER_FUNCTION_IDENTIFIER:
        case IDENTIFIER:
        case VARIABLE_IDENTIFIER:
            return std::make_shared<IdentifierToken>(static_cast<Tokens>(lex_val),
                                                     lexerToken.text,
                                                     lexerToken.token_row,
                                                     lexerToken.token_col,
                                                     lexerToken.token_leng,
                                                     lexerToken.identifier,
                                                     tComments);
        case STRING:
            return std::make_shared<StringToken>(static_cast<Tokens>(lex_val),
                                                 lexerToken.text,
                                                 lexerToken.token_row,
                                                 lexerToken.token_col,
                                                 lexerToken.token_leng,
                                                 lexerToken.string,
                                                 tComments);
        case REAL:
            return std::make_shared<RealToken>(static_cast<Tokens>(lex_val),
                                               lexerToken.text,
                                               lexerToken.token_row,
                                               lexerToken.token_col,
                                               lexerToken.token_leng,
                                               lexerToken.real,
                                               tComments);
        case INTEGER:
            return std::make_shared<IntegerToken>(static_cast<Tokens>(lex_val),
                                                  lexerToken.text,
                                                  lexerToken.token_row,
                                                  lexerToken.token_col,
                                                  lexerToken.token_leng,
                                                  lexerToken.integer,
                                                  tComments);
        case BOOLEAN:
            return std::make_shared<BooleanToken>(static_cast<Tokens>(lex_val),
                                                  lexerToken.text,
                                                  lexerToken.token_row,
                                                  lexerToken.token_col,
                                                  lexerToken.token_leng,
                                                  lexerToken.boolean,
                                                  tComments);
        case 0:
            return std::make_shared<AnnaToken>(END,
                                               lexerToken.text,
                                               lexerToken.token_row,
                                               lexerToken.token_col,
                                               lexerToken.token_leng,
                                               tComments);
        case ERROR:
            log_print_pos(lexerToken.token_row, lexerToken.token_col, __lex_filename);
            std::fprintf(__log_out, "Unrecognized token ``%s''\n", lexerToken.text->c_str());
            log_print_row(lexerToken.token_row);
            log_print_indicators(lexerToken.token_col, lexerToken.token_leng);
            return std::make_shared<AnnaToken>(ERROR,
                                               lexerToken.text,
                                               lexerToken.token_row,
                                               lexerToken.token_col,
                                               lexerToken.token_leng,
                                               tComments);
            break;
        default:
            throw;  // Should not happen
            break;
    }
}

void lexer_finalize()
{
    if (use_temp_file_stream)
        fclose(annain);
}

void log_print_row(int row)
{
    std::fputs(get_lex_source_row(row).c_str(), __log_out);
}

void log_print_row(int row, std::stringstream &logstream)
{
    logstream << get_lex_source_row(row).c_str();
}
