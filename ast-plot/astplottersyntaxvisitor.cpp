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


#include "astplottersyntaxvisitor.h"
#include "annasyntax.h"

ASTPlotterSyntaxVisitor::ASTPlotterSyntaxVisitor()
{
}

void ASTPlotterSyntaxVisitor::generateGraph(const std::string &dotFileName)
{
    std::ofstream dotfile(dotFileName);
    boost::write_graphviz(dotfile, graph, [this](std::ostream &out, auto &v) {
        NodeProperty node = graph[v];

        switch (node.type) {
            case NodeProperty::Syntax:
                out << "[label=\"" << node.name << "\" shape=box style=filled fillcolor=yellow]";
                break;
            case NodeProperty::Token:
                node.description = node.description.compare("\n") ? node.description : "\\n";
                out << "[label=\"" << node.name << "\n" << node.description
                    << "\" shape=diamond style=filled fillcolor=lightgreen]";
                break;
            case NodeProperty::Comment:
                out << "[label=\"" << node.name << "\n" << node.description
                    << "\" shape=note style=filled fillcolor=skyblue1]";
                break;
            default:
                break;
        }
    });
}

void ASTPlotterSyntaxVisitor::Visit(AnnaCompilationUnitSyntax &node)
{
    // Root
    ASTGraphDescriptor g = graph.add_vertex();
    graph[g].type = NodeProperty::Syntax;
    graph[g].name = "compilation unit";
    nodes.push(g);

    for (auto import : node.importDirectives)
        import->Accept(*this);

    for (auto var : node.variableDeclarationStatements)
        var->Accept(*this);

    for (auto funcDefs : node.functionDefinitions)
        funcDefs->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaFunctionBodySyntax &node)
{
    enterSyntaxNode("function body");

    node.block->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaExpressionSyntax &node)
{
    (void)node;
    throw;  // Abstract class
}

void ASTPlotterSyntaxVisitor::Visit(AnnaBinaryOperationExpressionSyntax &node)
{
    enterSyntaxNode("binary operation expression");

    node.left->Accept(*this);
    node.op->Accept(*this);
    node.right->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaUnaryExpressionSyntax &node)
{
    (void)node;
    throw;  // Abstract class
}

void ASTPlotterSyntaxVisitor::Visit(AnnaPrimaryExpressionSyntax &node)
{
    (void)node;
    throw; // Abstract class
}

void ASTPlotterSyntaxVisitor::Visit(AnnaSimpleNameSyntax &node)
{
    enterSyntaxNode("simple name");

    node.VARIABLE_IDENTIFIER->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaLiteralSyntax &node)
{
    enterSyntaxNode("literal");

    node.literal->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaParenthesizedExpressionSyntax &node)
{
    enterSyntaxNode("parenthesized expression");

    node.OPEN_PAREN->Accept(*this);
    node.expression->Accept(*this);
    node.CLOSE_PAREN->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaBinaryOperatorSyntax &node)
{
    enterSyntaxNode("binary operator");

    node.binOp->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaInvocationExpressionSyntax &node)
{
    enterSyntaxNode("invocation expression");

    node.functionIdentifier->Accept(*this);

    if (node.hasOptionalPar)
        node.OPEN_PAREN_opt->Accept(*this);

    if (node.hasArgs)
        node.argumentList->Accept(*this);

    if (node.hasOptionalPar)
        node.CLOSE_PAREN_opt->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaArgumentListSyntax &node)
{
    enterSyntaxNode("argument list");

    auto list = node.argumentList.list;
    for (auto couple : list) {
        couple.node->Accept(*this);
        if (couple.COMMA)
            couple.COMMA->Accept(*this);
    }

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaBlockSyntax &node)
{
    enterSyntaxNode("block");

    node.OPEN_BRACE->Accept(*this);
    for (auto stat : node.statements)
        stat->Accept(*this);
    node.CLOSE_BRACE->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaStatementSyntax &node)
{
    (void)node;
    throw; // Abstract class
}

void ASTPlotterSyntaxVisitor::Visit(AnnaEmbeddedStatementSyntax &node)
{
    (void)node;
    throw; // Abstract class
}

void ASTPlotterSyntaxVisitor::Visit(AnnaEmptyStatementSyntax &node)
{
    enterSyntaxNode("empty statement");

    if (node.has_eos)
        node.eos_opt->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaExpressionStatementSyntax &node)
{
    enterSyntaxNode("expression statement");

    node.statementExpression->Accept(*this);
    node.eos->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(StringToken &node)
{
    createTokenNode("String", node.row(), node.col(), *node.string(), node.trailingComments());
}

void ASTPlotterSyntaxVisitor::Visit(LiteralToken &node)
{
    (void)node;
    throw; // Abstract class
}

void ASTPlotterSyntaxVisitor::Visit(BooleanToken &node)
{
    createTokenNode("Boolean", node.row(), node.col(), node.boolean() ? "true" : "false", node.trailingComments());
}

void ASTPlotterSyntaxVisitor::Visit(IntegerToken &node)
{
    createTokenNode("Integer", node.row(), node.col(), std::to_string(node.integer()), node.trailingComments());
}

void ASTPlotterSyntaxVisitor::Visit(RealToken &node)
{
    createTokenNode("Real", node.row(), node.col(), std::to_string(node.real()), node.trailingComments());
}

void ASTPlotterSyntaxVisitor::Visit(IdentifierToken &node)
{
    createTokenNode("Identifier", node.row(), node.col(), *node.identifier(), node.trailingComments());
}

void ASTPlotterSyntaxVisitor::Visit(AnnaToken &node)
{
    createTokenNode("Token", node.row(), node.col(), *node.text(), node.trailingComments());
}

void ASTPlotterSyntaxVisitor::Visit(AnnaEOSSyntax &node)
{
    enterSyntaxNode("eos");

    for (auto t : node.T)
        t->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaReturnStatementSyntax &node)
{
    enterSyntaxNode("return statement");

    node.RETURN->Accept(*this);
    if (node.hasExpr)
        node.expression->Accept(*this);
    node.eos->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaAssignmentSyntax &node)
{
    enterSyntaxNode("assignment");

    node.left->Accept(*this);
    node.EQ->Accept(*this);
    node.right->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaWhileStatementSyntax &node)
{
    enterSyntaxNode("while statement");

    node.WHILE->Accept(*this);
    node.OPEN_PAREN->Accept(*this);
    node.condition->Accept(*this);
    node.CLOSE_PAREN->Accept(*this);
    node.while_body->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaIterationStatementSyntax &node)
{
    (void)node;
    throw; // Abstract class
}

void ASTPlotterSyntaxVisitor::Visit(AnnaIfStatementSyntax &node)
{
    enterSyntaxNode("if statement");

    node.IF->Accept(*this);
    node.OPEN_PAREN->Accept(*this);
    node.condition->Accept(*this);
    node.CLOSE_PAREN->Accept(*this);
    node.embeddedStatement->Accept(*this);

    if (node.hasElse) {
        node.ELSE_opt->Accept(*this);
        node.elseStatement_opt->Accept(*this);
    }

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaSelectionStatementSyntax &node)
{
    (void)node;
    throw; // Abstract class
}

void ASTPlotterSyntaxVisitor::Visit(AnnaStatementExpressionSyntax &node)
{
    (void)node;
    throw; // Abstract class
}

void ASTPlotterSyntaxVisitor::Visit(AnnaFormalParameterSyntax &node)
{
    enterSyntaxNode("formal parameter");

    node.VARIABLE_IDENTIFIER->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaFormalParameterListSyntax &node)
{
    enterSyntaxNode("formal parameter list");

    for (size_t i = 0; i < node.formalParameterList.list.size(); ++i) {
        auto list = node.formalParameterList.list;

        for (auto couple : list) {
            couple.node->Accept(*this);
            if (couple.COMMA)
                couple.COMMA->Accept(*this);
        }
    }

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaFunctionHeaderSyntax &node)
{
    enterSyntaxNode("function header");

    node.DEF->Accept(*this);
    node.USER_FUNCTION_IDENTIFIER->Accept(*this);
    node.OPEN_PAREN->Accept(*this);
    if (node.hasParameter) {
        node.formalParameterList_opt->Accept(*this);
    }
    node.CLOSE_PAREN->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaFunctionDefinitionSyntax &node)
{
    enterSyntaxNode("function definition");

    node.functionHeader->Accept(*this);
    node.functionBody->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaFunctionIdentifierSyntax &node)
{
    enterSyntaxNode("function identifier");

    node.identifier->Accept(*this);

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaVariableDeclarationStatementSyntax &node)
{
    enterSyntaxNode("variable declaration statement");

    node.VAR->Accept(*this);
    node.VARIABLE_IDENTIFIER->Accept(*this);;
    node.EOS->Accept(*this);

    if (node.hasAssignment) {
        node.EQ_opt->Accept(*this);
        node.primaryExpression_opt->Accept(*this);
    }

    exitNode();
}

void ASTPlotterSyntaxVisitor::Visit(AnnaImportDirectiveSyntax &node)
{
    enterSyntaxNode("import");

    node.IMPORT->Accept(*this);
    node.IDENTIFIER->Accept(*this);
    node.eos->Accept(*this);

    exitNode();
}




























