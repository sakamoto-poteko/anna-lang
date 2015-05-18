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
#include "annatoken.h"
#include "annasyntax.h"

#include <stack>

class AnnaParser
{
public:
    AnnaParser(FILE *in);
    AnnaParser(char *text, size_t len);
    virtual ~AnnaParser();

    // FIXME: Change reutrn type
    static void ParseText(gcString text);

    gcnCompilationUnit parse();

protected:
    bool lexall();

    gcnEOS parseEOS();
    gcnCompilationUnit parseCompilationUnit();
    gcnImportDirective parseImportDirective();
    gcnFunctionIdentifier parseFunctionIdentifier();
    gcnExpression parseExpression();
    gcnBinaryOperationExpression parseBinaryOperationExpression();
    gcnUnaryExpression parseUnaryExpression();
    gcnPrimaryExpression parsePrimaryExpression();
    gcnSimpleName parseSimpleName();
    gcnLiteral parseLiteral();
    gcnParenthesizedExpression parseParenthesizedExpression();
    gcnBinaryOperator parseBinaryOperator();
    gcnInvocationExpression parseInvocationExpression();
    gcnArgumentList parseArgumentList();
    gcnFunctionDefinition parseFunctionDefinition();
    gcnFunctionHeader parseFunctionHeader();
    gcnFormalParameterList parseFormalParameterList();
    gcnFormalParameter parseFormalParameter();
    gcnFunctionBody parseFunctionBody();
    gcnBlock parseBlock();
    gcnStatement parseStatement();
    gcnEmbeddedStatement parseEmbeddedStatement();
    gcnVariableDeclarationStatement parseVariableDeclarationStatement();
    gcnEmptyStatement parseEmptyStatement();
    gcnExpressionStatement parseExpressionStatement();
    gcnStatementExpression parseStatementExpression();
    gcnSelectionStatement parseSelectionStatement();
    gcnIfStatement parseIfStatement();
    gcnIterationStatement parseIterationStatement();
    gcnWhileStatement parseWhileStatement();
    gcnAssignment parseAssignment();


    std::vector<gcnToken> tokens;
    unsigned int currentTokenIdx = 0;
    gcnToken currentToken;

    gcnToken eatToken(bool requireT = false);

    // Always use this signature. When there's a kind mismatch,
    // no token will be consumed.
    gcnToken eatToken(Tokens kind, const char *expected = 0);

    gcnToken peekToken(int ahead);
    void revertToken(unsigned int index);


    std::stack<unsigned int> tokenIdxStack;
    void pushTokenStatus()
    {
        tokenIdxStack.push(currentTokenIdx);
    }

    void popTokenStatus()
    {
        tokenIdxStack.pop();
    }

    void revertTokenStatus()
    {
        revertToken(tokenIdxStack.top());
        popTokenStatus();
    }
};



#endif // PARSER_H
