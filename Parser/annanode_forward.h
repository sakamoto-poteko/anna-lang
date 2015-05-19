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


#ifndef ANNANODE_FORWARD
#define ANNANODE_FORWARD

#include <memory>

// Base
class AnnaNode;

// Syntax Nodes
class AnnaEOSSyntax;
class AnnaCompilationUnitSyntax;
class AnnaImportDirectiveSyntax;
class AnnaFunctionIdentifierSyntax;
class AnnaExpressionSyntax;
class AnnaBinaryOperationExpressionSyntax;
class AnnaUnaryExpressionSyntax;
class AnnaPrimaryExpressionSyntax;
class AnnaSimpleNameSyntax;
class AnnaLiteralSyntax;
class AnnaParenthesizedExpressionSyntax;
class AnnaBinaryOperatorSyntax;
class AnnaInvocationExpressionSyntax;
class AnnaArgumentListSyntax;
class AnnaFunctionDefinitionSyntax;
class AnnaFunctionHeaderSyntax;
class AnnaFormalParameterListSyntax;
class AnnaFunctionBodySyntax;
class AnnaBlockSyntax;
class AnnaStatementSyntax;
class AnnaEmbeddedStatementSyntax;
class AnnaVariableDeclarationStatementSyntax;
class AnnaEmptyStatementSyntax;
class AnnaExpressionStatementSyntax;
class AnnaStatementExpressionSyntax;
class AnnaSelectionStatementSyntax;
class AnnaIfStatementSyntax;
class AnnaIterationStatementSyntax;
class AnnaWhileStatementSyntax;
class AnnaAssignmentSyntax;
class AnnaFormalParameterSyntax;
class AnnaReturnStatementSyntax;

typedef std::shared_ptr<AnnaEOSSyntax> gcnEOS;
typedef std::shared_ptr<AnnaCompilationUnitSyntax> gcnCompilationUnit;
typedef std::shared_ptr<AnnaImportDirectiveSyntax> gcnImportDirective;
typedef std::shared_ptr<AnnaFunctionIdentifierSyntax> gcnFunctionIdentifier;
typedef std::shared_ptr<AnnaExpressionSyntax> gcnExpression;
typedef std::shared_ptr<AnnaBinaryOperationExpressionSyntax> gcnBinaryOperationExpression;
typedef std::shared_ptr<AnnaUnaryExpressionSyntax> gcnUnaryExpression;
typedef std::shared_ptr<AnnaPrimaryExpressionSyntax> gcnPrimaryExpression;
typedef std::shared_ptr<AnnaSimpleNameSyntax> gcnSimpleName;
typedef std::shared_ptr<AnnaLiteralSyntax> gcnLiteral;
typedef std::shared_ptr<AnnaParenthesizedExpressionSyntax> gcnParenthesizedExpression;
typedef std::shared_ptr<AnnaBinaryOperatorSyntax> gcnBinaryOperator;
typedef std::shared_ptr<AnnaInvocationExpressionSyntax> gcnInvocationExpression;
typedef std::shared_ptr<AnnaArgumentListSyntax> gcnArgumentList;
typedef std::shared_ptr<AnnaFunctionDefinitionSyntax> gcnFunctionDefinition;
typedef std::shared_ptr<AnnaFunctionHeaderSyntax> gcnFunctionHeader;
typedef std::shared_ptr<AnnaFormalParameterListSyntax> gcnFormalParameterList;
typedef std::shared_ptr<AnnaFunctionBodySyntax> gcnFunctionBody;
typedef std::shared_ptr<AnnaBlockSyntax> gcnBlock;
typedef std::shared_ptr<AnnaStatementSyntax> gcnStatement;
typedef std::shared_ptr<AnnaEmbeddedStatementSyntax> gcnEmbeddedStatement;
typedef std::shared_ptr<AnnaVariableDeclarationStatementSyntax> gcnVariableDeclarationStatement;
typedef std::shared_ptr<AnnaEmptyStatementSyntax> gcnEmptyStatement;
typedef std::shared_ptr<AnnaExpressionStatementSyntax> gcnExpressionStatement;
typedef std::shared_ptr<AnnaStatementExpressionSyntax> gcnStatementExpression;
typedef std::shared_ptr<AnnaSelectionStatementSyntax> gcnSelectionStatement;
typedef std::shared_ptr<AnnaIfStatementSyntax> gcnIfStatement;
typedef std::shared_ptr<AnnaIterationStatementSyntax> gcnIterationStatement;
typedef std::shared_ptr<AnnaWhileStatementSyntax> gcnWhileStatement;
typedef std::shared_ptr<AnnaAssignmentSyntax> gcnAssignment;
typedef std::shared_ptr<AnnaFormalParameterSyntax> gcnFormalParameter;
typedef std::shared_ptr<AnnaReturnStatementSyntax> gcnReturnStatement;

// Tokens
class AnnaToken;
class IdentifierToken;
class RealToken;
class IntegerToken;
class BooleanToken;
class StringToken;
class LiteralToken;

typedef std::shared_ptr<AnnaToken>          gcnToken;
typedef std::shared_ptr<IdentifierToken>    gcnIdentifierToken;
typedef std::shared_ptr<RealToken>          gcnRealToken;
typedef std::shared_ptr<IntegerToken>       gcnIntegerToken;
typedef std::shared_ptr<BooleanToken>       gcnBooleanToken;
typedef std::shared_ptr<StringToken>        gcnStringToken;
typedef std::shared_ptr<LiteralToken>       gcnLiteralToken;


#endif // ANNANODE_FORWARD

