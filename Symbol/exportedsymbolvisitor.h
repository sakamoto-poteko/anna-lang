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


#ifndef EXPORTEDSYMBOLVISITOR_H
#define EXPORTEDSYMBOLVISITOR_H

#include "symbol.h"
#include "annasyntaxvisitor.h"
#include "compilationunitsymbolcollection.h"

class ExportedSymbolVisitor : public AnnaSyntaxVisitor
{
public:
    ExportedSymbolVisitor();
    ~ExportedSymbolVisitor();

    // Syntax Nodes
    virtual void Visit(AnnaEOSSyntax &node);
    virtual void Visit(AnnaCompilationUnitSyntax &node);
    virtual void Visit(AnnaImportDirectiveSyntax &node);
    virtual void Visit(AnnaFunctionIdentifierSyntax &node);
    virtual void Visit(AnnaExpressionSyntax &node);
    virtual void Visit(AnnaBinaryOperationExpressionSyntax &node);
    virtual void Visit(AnnaUnaryExpressionSyntax &node);
    virtual void Visit(AnnaPrimaryExpressionSyntax &node);
    virtual void Visit(AnnaSimpleNameSyntax &node);
    virtual void Visit(AnnaLiteralSyntax &node);
    virtual void Visit(AnnaParenthesizedExpressionSyntax &node);
    virtual void Visit(AnnaBinaryOperatorSyntax &node);
    virtual void Visit(AnnaInvocationExpressionSyntax &node);
    virtual void Visit(AnnaArgumentListSyntax &node);
    virtual void Visit(AnnaFunctionDefinitionSyntax &node);
    virtual void Visit(AnnaFunctionHeaderSyntax &node);
    virtual void Visit(AnnaFormalParameterListSyntax &node);
    virtual void Visit(AnnaFunctionBodySyntax &node);
    virtual void Visit(AnnaBlockSyntax &node);
    virtual void Visit(AnnaStatementSyntax &node);
    virtual void Visit(AnnaEmbeddedStatementSyntax &node);
    virtual void Visit(AnnaVariableDeclarationStatementSyntax &node);
    virtual void Visit(AnnaEmptyStatementSyntax &node);
    virtual void Visit(AnnaExpressionStatementSyntax &node);
    virtual void Visit(AnnaStatementExpressionSyntax &node);
    virtual void Visit(AnnaSelectionStatementSyntax &node);
    virtual void Visit(AnnaIfStatementSyntax &node);
    virtual void Visit(AnnaIterationStatementSyntax &node);
    virtual void Visit(AnnaWhileStatementSyntax &node);
    virtual void Visit(AnnaAssignmentSyntax &node);
    virtual void Visit(AnnaFormalParameterSyntax &node);
    virtual void Visit(AnnaReturnStatementSyntax &node);

    // Tokens
    virtual void Visit(AnnaToken &node);
    virtual void Visit(IdentifierToken &node);
    virtual void Visit(RealToken &node);
    virtual void Visit(IntegerToken &node);
    virtual void Visit(BooleanToken &node);
    virtual void Visit(StringToken &node);
    virtual void Visit(LiteralToken &node);

    CompilationUnitSymbolCollection symbols() { return _symbols; }

protected:
    CompilationUnitSymbolCollection _symbols;

};

#endif // EXPORTEDSYMBOLVISITOR_H
