#include <stdio.h>
#include "lex_helper.h"


int main()
{
    FILE *f = fopen("/home/afa/sample.anna", "r");

    fseek(f, 0, SEEK_END);
    size_t fileLen=ftell(f);
    fseek(f, 0, SEEK_SET);
    char *content = new char[fileLen];
    fread(content, 1, fileLen, f);

    //lexer_init(f);
    lexer_init(content, fileLen);

    std::vector<gcToken> tokens;

    gcToken token = lex();
    while (token->token() >0) {
        tokens.push_back(token);
        token = lex();
    }
    if (token->token() == ERROR) {
        return -1;
    }

    for (size_t i = 0; i < tokens.size(); ++i) {
        printf("%s\n", tokens[i]->text()->c_str());
    }


    lexer_finalize();
    delete content;
    return 0;
}

