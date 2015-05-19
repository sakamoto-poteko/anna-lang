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

#include "annasyntax.h"

void AnnaEOSSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaCompilationUnitSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaImportDirectiveSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaFunctionIdentifierSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaBinaryOperationExpressionSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaSimpleNameSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaLiteralSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaParenthesizedExpressionSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaBinaryOperatorSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaInvocationExpressionSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaArgumentListSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaFunctionDefinitionSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaFunctionHeaderSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaFormalParameterListSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaFunctionBodySyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaBlockSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaVariableDeclarationStatementSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaEmptyStatementSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaExpressionStatementSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaIfStatementSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaWhileStatementSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaAssignmentSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaFormalParameterSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}

void AnnaReturnStatementSyntax::Accept(AnnaSyntaxVisitor &visitor)
{
    visitor.Visit(*this);
}
