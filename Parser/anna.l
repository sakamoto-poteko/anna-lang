%{
/**************************************************************************
 * Copyright (c) 2015 Afa.L Cheng <afa@afa.moe>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software "), to deal in the Software without restriction, including
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

#include "lex_helper.h"
#include "lexertoken.h"
#include "parser.h"

static std::string COMS_str;
static int current_row;
static int current_column;
static std::vector<std::string> source_rows;

void advance_token()
{
//    lexerToken.clear(); // moved to tokenize()
    lexerToken.token_row = current_row;
    lexerToken.token_col = current_column;
    lexerToken.token_leng = annaleng;
    lexerToken.text = std::make_shared<std::string>(annatext);
    current_column += annaleng;
}

void advance_row()
{
    advance_token(); ++current_row; current_column = 0;
}

std::string get_lex_source_row(int row)
{
    return source_rows[row];
}

std::vector<std::string> &get_lex_source_rows()
{
    return source_rows;
}

%}

%option nounistd
%option never-interactive
%option noyywrap
%x COMS1 COMS2 COMS3

%%
"def"                           { advance_token(); return DEF;     }
"main"                          { advance_token(); return MAIN;    }
"import"                        { advance_token(); return IMPORT;  }
"return"                        { advance_token(); return RETURN;  }
"var"                           { advance_token(); return VAR;     }


"if"                            { advance_token(); return IF;      }
"else"                          { advance_token(); return ELSE;    }
"while"                         { advance_token(); return WHILE;   }

">="                            { advance_token(); return GE;      }
"<="                            { advance_token(); return LE;      }
"=="                            { advance_token(); return EE;      }
"!="                            { advance_token(); return NE;      }
">"                             { advance_token(); return GT;           }
"<"                             { advance_token(); return LT;           }

"("                             { advance_token(); return OPEN_PAREN;   }
")"                             { advance_token(); return CLOSE_PAREN;  }
"{"                             { advance_token(); return OPEN_BRACE;   }
"}"                             { advance_token(); return CLOSE_BRACE;  }
"["                             { advance_token(); return OPEN_BRACKET; }
"]"                             { advance_token(); return CLOSE_BRACKET;}
","                             { advance_token(); return COMMA;        }

"&&"                            { advance_token(); return ANDAND;       }
"||"                            { advance_token(); return OROR;         }
"&"                             { advance_token(); return AND;          }
"|"                             { advance_token(); return OR;           }
"^"                             { advance_token(); return XOR;          }
"+"                             { advance_token(); return ADD;          }
"-"                             { advance_token(); return SUB;          }
"*"                             { advance_token(); return MUL;          }
"/"                             { advance_token(); return DIV;          }
"%"                             { advance_token(); return MOD;          }
"!"                             { advance_token(); return NOT;          }
"~"                             { advance_token(); return BITNOT;       }
"="                             { advance_token(); return EQ;           }
";"                             { advance_token(); return T;            }

"true"                          { advance_token(); lexerToken.boolean = 1; return BOOLEAN; }
"false"                         { advance_token(); lexerToken.boolean = 0; return BOOLEAN; }
[0-9]+\.[0-9]*                  { advance_token(); lexerToken.real = atof(annatext);     return REAL;    }
[0-9]+                          { advance_token(); lexerToken.integer = atoi(annatext);  return INTEGER; }
\"([^\\\"]|\\.)*\"              { advance_token(); lexerToken.string = std::make_shared<std::string>(annatext); return STRING;}

"50USD"                         { advance_token(); lexerToken.identifier = std::make_shared<std::string>(annatext); return VARIABLE_IDENTIFIER; }
"500USD"                        { advance_token(); lexerToken.identifier = std::make_shared<std::string>(annatext); return VARIABLE_IDENTIFIER; }
a`[0-9]+                        { advance_token(); lexerToken.identifier = std::make_shared<std::string>(annatext); return VARIABLE_IDENTIFIER; }
an+a                            { advance_token(); lexerToken.identifier = std::make_shared<std::string>(annatext); return VARIABLE_IDENTIFIER; }
@[A-Za-z][A-Za-z0-9_]*          { advance_token(); lexerToken.identifier = std::make_shared<std::string>(annatext); return USER_FUNCTION_IDENTIFIER; }
[A-Za-z_][A-Za-z0-9_]*          { advance_token(); lexerToken.identifier = std::make_shared<std::string>(annatext); return IDENTIFIER;          }
"\n"                            { advance_row(); return T;      }
[ \t\r]+                        { advance_token(); }
.                               { advance_token(); return ERROR; }

"-_-"                           { advance_token(); COMS_str.clear(); COMS_str.append(annatext); BEGIN(COMS1);  }
<COMS1>">"                      { advance_token(); COMS_str.append(annatext); BEGIN(COMS2);  }
<COMS1>"\n"                     { advance_row();   COMS_str.append(annatext); }
<COMS1>[^>\n]                   { advance_token(); COMS_str.append(annatext); }
<COMS2>"_"                      { advance_token(); COMS_str.append(annatext); BEGIN(COMS3);  }
<COMS2>"\n"                     { advance_row();   COMS_str.append(annatext); BEGIN(COMS1);  }
<COMS2>[^_\n]                   { advance_token(); COMS_str.append(annatext); BEGIN(COMS1);  }
<COMS3>"<"                      { advance_token(); COMS_str.append(annatext); lexerToken.trailing_comments.push_back(COMS_str); BEGIN(INITIAL); }
<COMS3>"\n"                     { advance_token(); COMS_str.append(annatext); BEGIN(COMS1);  }
<COMS3>[^<\n]                   { advance_row();   COMS_str.append(annatext); BEGIN(COMS1);  }

%%
