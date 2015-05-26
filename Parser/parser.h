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
#include <sstream>
#include <iostream>

class AnnaParser
{
public:
    AnnaParser(FILE *in, const std::string &fileName, const std::string compilationUnitName);
    AnnaParser(char *text, size_t len, const std::string &fileName, const std::string compilationUnitName);
    virtual ~AnnaParser();

    // FIXME: Change reutrn type
    static void ParseText(gcString text);

    gcnCompilationUnit parse();

    void printErrors()
    {
        while (!errorStreams.empty()) {
            std::cout << errorStreams.top()->str();
            errorStreams.pop();
        }
    }

protected:
    bool lexall();

    gcnEOS parseEOS();
    gcnCompilationUnit parseCompilationUnit();
    gcnImportDirective parseImportDirective();
    gcnFunctionIdentifier parseFunctionIdentifier();
    gcnExpression parseExpression();
    gcnBinaryOperationExpression parseBinaryOperationExpression(int min_precedence = 0);
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
    gcnReturnStatement parseReturnStatement();

    int getPrecedence(Tokens op);
    bool isLeftAssociative(Tokens op);


    std::vector<gcnToken> tokens;
    unsigned int currentTokenIdx = 0;
    gcnToken currentToken;

    gcnToken eatToken(bool dontIgnoreNewlineT = false);

    // Always use this signature. When there's a kind mismatch,
    // no token will be consumed.
    gcnToken eatToken(Tokens kind, const char *expected = 0, const char *caller = "", bool dontIgnoreNewlineT = false);

    gcnToken peekToken(int ahead = 0, bool dontIgnoreNewlineT = false);
    void revertToken(unsigned int index);


    // Parser status stacks
    std::stack<unsigned int> tokenIdxStack;
    std::stack<std::shared_ptr<std::stringstream>> errorStreams;

    std::stringstream &currentErrorStream()
    {
        return *errorStreams.top();
    }

    void pushParserStatus()
    {
        errorStreams.push(std::make_shared<std::stringstream>());
        tokenIdxStack.push(currentTokenIdx);
    }

    void popParserStatus()
    {
        errorStreams.pop();
        tokenIdxStack.pop();
    }

    void revertParserStatus()
    {
        std::string err = errorStreams.top()->str();
        errorStreams.pop();
        *errorStreams.top() << err;
        revertToken(tokenIdxStack.top());
        tokenIdxStack.pop();
    }


    bool isNewlineT(const gcnToken &token);



    std::string _filename;
    gcString _compilationUnitName;
    bool isPossiblePrimaryExpression();
};



#endif // PARSER_H
