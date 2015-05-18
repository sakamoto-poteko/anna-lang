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


#ifndef ANNASYNTAXVISITOR_H
#define ANNASYNTAXVISITOR_H

#include "annanode_forward.h"

class AnnaSyntaxVisitor
{
public:
    virtual void Visit(AnnaNode &node);

    // Syntax Nodes
    virtual void Visit(AnnaEOSSyntax &node) = 0;
    virtual void Visit(AnnaCompilationUnitSyntax &node) = 0;
    virtual void Visit(AnnaImportDirectiveSyntax &node) = 0;
    virtual void Visit(AnnaFunctionIdentifierSyntax &node) = 0;
    virtual void Visit(AnnaExpressionSyntax &node) = 0;
    virtual void Visit(AnnaBinaryOperationExpressionSyntax &node) = 0;
    virtual void Visit(AnnaUnaryExpressionSyntax &node) = 0;
    virtual void Visit(AnnaPrimaryExpressionSyntax &node) = 0;
    virtual void Visit(AnnaSimpleNameSyntax &node) = 0;
    virtual void Visit(AnnaLiteralSyntax &node) = 0;
    virtual void Visit(AnnaParenthesizedExpressionSyntax &node) = 0;
    virtual void Visit(AnnaBinaryOperatorSyntax &node) = 0;
    virtual void Visit(AnnaInvocationExpressionSyntax &node) = 0;
    virtual void Visit(AnnaArgumentListSyntax &node) = 0;
    virtual void Visit(AnnaFunctionDefinitionSyntax &node) = 0;
    virtual void Visit(AnnaFunctionHeaderSyntax &node) = 0;
    virtual void Visit(AnnaFormalParameterListSyntax &node) = 0;
    virtual void Visit(AnnaFunctionBodySyntax &node) = 0;
    virtual void Visit(AnnaBlockSyntax &node) = 0;
    virtual void Visit(AnnaStatementSyntax &node) = 0;
    virtual void Visit(AnnaEmbeddedStatementSyntax &node) = 0;
    virtual void Visit(AnnaVariableDeclarationStatementSyntax &node) = 0;
    virtual void Visit(AnnaEmptyStatementSyntax &node) = 0;
    virtual void Visit(AnnaExpressionStatementSyntax &node) = 0;
    virtual void Visit(AnnaStatementExpressionSyntax &node) = 0;
    virtual void Visit(AnnaSelectionStatementSyntax &node) = 0;
    virtual void Visit(AnnaIfStatementSyntax &node) = 0;
    virtual void Visit(AnnaIterationStatementSyntax &node) = 0;
    virtual void Visit(AnnaWhileStatementSyntax &node) = 0;
    virtual void Visit(AnnaAssignmentSyntax &node) = 0;
    virtual void Visit(AnnaFormalParameterSyntax &node) = 0;

    // Tokens
    virtual void Visit(AnnaToken &node) = 0;
    virtual void Visit(IdentifierToken &node) = 0;
    virtual void Visit(RealToken &node) = 0;
    virtual void Visit(IntegerToken &node) = 0;
    virtual void Visit(BooleanToken &node) = 0;
    virtual void Visit(StringToken &node) = 0;
    virtual void Visit(LiteralToken &node) = 0;

protected:
    AnnaSyntaxVisitor();
};

#endif // ANNASYNTAXVISITOR_H
