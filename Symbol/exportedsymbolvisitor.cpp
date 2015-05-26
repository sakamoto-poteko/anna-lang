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

#include <cassert>

#include "exportedsymbolvisitor.h"
#include "annasyntax.h"

ExportedSymbolVisitor::ExportedSymbolVisitor()
{

}

ExportedSymbolVisitor::~ExportedSymbolVisitor()
{

}


void ExportedSymbolVisitor::Visit(AnnaCompilationUnitSyntax &node)
{
    for (auto var : node.variableDeclarationStatements) {
        gcVariableDeclarationSymbol varSymbol = std::make_shared<VariableDeclarationSymbol>();

        varSymbol->name = var->VARIABLE_IDENTIFIER->identifier();
        _symbols.globals.push_back(varSymbol);
    }

    for (auto funcDef : node.functionDefinitions) {
        gcFunctionDefinitionSymbol funcSymbol = std::make_shared<FunctionDefinitionSymbol>();
        gcnFunctionHeader header = funcDef->functionHeader;
        funcSymbol->name = header->USER_FUNCTION_IDENTIFIER->identifier();
        if (header->hasParameter)
            funcSymbol->paramsCount = header->formalParameterList_opt->formalParameterList.list.size();

        _symbols.functions.push_back(funcSymbol);
    }

    _symbols.compilationUnitName = node.compilationUnitName;
}

void ExportedSymbolVisitor::Visit(AnnaFunctionBodySyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaExpressionSyntax &node)
{
    (void)node;
    throw;  // Abstract class
}

void ExportedSymbolVisitor::Visit(AnnaBinaryOperationExpressionSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaUnaryExpressionSyntax &node)
{
    (void)node;
    throw;  // Abstract class
}

void ExportedSymbolVisitor::Visit(AnnaPrimaryExpressionSyntax &node)
{
    (void)node;
    throw; // Abstract class
}

void ExportedSymbolVisitor::Visit(AnnaSimpleNameSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaLiteralSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaParenthesizedExpressionSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaBinaryOperatorSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaInvocationExpressionSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaArgumentListSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaBlockSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaStatementSyntax &node)
{
    (void)node;
    throw; // Abstract class
}

void ExportedSymbolVisitor::Visit(AnnaEmbeddedStatementSyntax &node)
{
    (void)node;
    throw; // Abstract class
}

void ExportedSymbolVisitor::Visit(AnnaEmptyStatementSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaExpressionStatementSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(StringToken &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(LiteralToken &node)
{
    (void)node;
    throw; // Abstract class
}

void ExportedSymbolVisitor::Visit(BooleanToken &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(IntegerToken &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(RealToken &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(IdentifierToken &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaToken &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaEOSSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaReturnStatementSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaAssignmentSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaWhileStatementSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaIterationStatementSyntax &node)
{
    (void)node;
    throw; // Abstract class
}

void ExportedSymbolVisitor::Visit(AnnaIfStatementSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaSelectionStatementSyntax &node)
{
    (void)node;
    throw; // Abstract class
}

void ExportedSymbolVisitor::Visit(AnnaStatementExpressionSyntax &node)
{
    (void)node;
    throw; // Abstract class
}

void ExportedSymbolVisitor::Visit(AnnaFormalParameterSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaFormalParameterListSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaFunctionHeaderSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaFunctionDefinitionSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaFunctionIdentifierSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaVariableDeclarationStatementSyntax &node)
{
    (void)node;
    assert(false);
}

void ExportedSymbolVisitor::Visit(AnnaImportDirectiveSyntax &node)
{
    (void)node;
    assert(false);
}

