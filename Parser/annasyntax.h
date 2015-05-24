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


#ifndef ANNASYNTAX_H
#define ANNASYNTAX_H

#include "annatoken.h"
#include "annanode.h"
#include "annasyntaxvisitor.h"
#include "annanode_forward.h"
#include <type_traits>

////////////////////
// Non-Terminals //
///////////////////
class AnnaSyntax : public AnnaNode
{
public:

protected:

};

////////////
// Helper //
////////////

template <class T>
class AnnaSeperatedList
{
public:
    AnnaSeperatedList()
    {
        static_assert(std::is_base_of<AnnaSyntax, class T::element_type>::value, "AnnaSeperatedList takes only AnnaSyntax");
    }

    void add(T node)
    {
        Couple n;
        n.node = node;
        list.push_back(n);
    }

    void addSeperator(gcnToken seperator)
    {
        if (list.empty() || list.back().COMMA) {
            Couple n;
            n.COMMA = seperator;
            list.push_back(n);
        } else {
            list.back().COMMA = seperator;
        }
    }

//protected:
    struct Couple {
        T node;
        gcnToken COMMA;
    };

    std::vector<Couple> list;
};

////////////
// Common //
////////////

class AnnaEOSSyntax : public AnnaSyntax
{
public:
    AnnaEOSSyntax(const std::vector<gcnToken> &Ts) : T(Ts) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    std::vector<gcnToken> T;
};

class AnnaCompilationUnitSyntax : public AnnaSyntax
{
public:
    AnnaCompilationUnitSyntax(const std::vector<gcnImportDirective> &imp,
                              const std::vector<gcnVariableDeclarationStatement> &vard,
                              const std::vector<gcnFunctionDefinition> &funcd) :
        importDirectives(imp), variableDeclarationStatements(vard), functionDefinitions(funcd)
    {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    std::vector<gcnImportDirective> importDirectives;
    std::vector<gcnVariableDeclarationStatement> variableDeclarationStatements;
    std::vector<gcnFunctionDefinition> functionDefinitions;
};

class AnnaImportDirectiveSyntax : public AnnaSyntax
{
public:
    AnnaImportDirectiveSyntax(gcnToken import, gcnIdentifierToken id, gcnEOS e) :
        IMPORT(import), IDENTIFIER(id), eos(e)
    {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnToken IMPORT;
    gcnIdentifierToken IDENTIFIER;
    gcnEOS eos;
};

//////////////
// Function //
//////////////
class AnnaFunctionDefinitionSyntax : public AnnaSyntax
{
public:
    AnnaFunctionDefinitionSyntax(gcnFunctionHeader header, gcnFunctionBody body) :
        functionHeader(header), functionBody(body) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnFunctionHeader functionHeader;
    gcnFunctionBody functionBody;
};

class AnnaFunctionHeaderSyntax : public AnnaSyntax
{
public:
    AnnaFunctionHeaderSyntax(gcnToken def, gcnIdentifierToken id,
                             gcnToken openPar, gcnFormalParameterList pm,
                             gcnToken closePar) :
        DEF(def), USER_FUNCTION_IDENTIFIER(id), OPEN_PAREN(openPar),
        formalParameterList_opt(pm), CLOSE_PAREN(closePar), hasParameter(true)
    {}

    AnnaFunctionHeaderSyntax(gcnToken def, gcnIdentifierToken id,
                             gcnToken openPar,
                             gcnToken closePar) :
        DEF(def), USER_FUNCTION_IDENTIFIER(id), OPEN_PAREN(openPar),
        CLOSE_PAREN(closePar), hasParameter(false) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnToken DEF;
    gcnIdentifierToken USER_FUNCTION_IDENTIFIER;
    gcnToken OPEN_PAREN;
    gcnFormalParameterList formalParameterList_opt;
    gcnToken CLOSE_PAREN;

    bool hasParameter;
};


class AnnaFormalParameterSyntax : public AnnaSyntax
{
public:
    AnnaFormalParameterSyntax(gcnIdentifierToken id) : VARIABLE_IDENTIFIER(id) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnIdentifierToken VARIABLE_IDENTIFIER;
};

class AnnaFormalParameterListSyntax : public AnnaSyntax
{
public:
    AnnaFormalParameterListSyntax(AnnaSeperatedList<gcnFormalParameter> list) :
        formalParameterList(list) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    AnnaSeperatedList<gcnFormalParameter> formalParameterList;
};

class AnnaFunctionBodySyntax : public AnnaSyntax
{
public:
    AnnaFunctionBodySyntax(gcnBlock bl) : block(bl) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnBlock block;
};

class AnnaFunctionIdentifierSyntax : public AnnaSyntax
{
public:
    AnnaFunctionIdentifierSyntax(gcnIdentifierToken id) : identifier(id) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnIdentifierToken identifier;
};

////////////////
// Expression //
////////////////
class AnnaExpressionSyntax : public AnnaSyntax
{
public:

protected:
    AnnaExpressionSyntax() {}
};

class AnnaBinaryOperationExpressionSyntax : public AnnaExpressionSyntax
{
public:
    AnnaBinaryOperationExpressionSyntax(gcnExpression l, gcnBinaryOperator o,
                                        gcnExpression r) :
        left(l), op(o), right(r) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnExpression left;
    gcnBinaryOperator op;
    gcnExpression right;
};

class AnnaUnaryExpressionSyntax : public AnnaExpressionSyntax
{
public:
protected:
    AnnaUnaryExpressionSyntax() {}
};

class AnnaPrimaryExpressionSyntax : public AnnaUnaryExpressionSyntax
{
public:
protected:
    AnnaPrimaryExpressionSyntax() {}
};

class AnnaSimpleNameSyntax : public AnnaPrimaryExpressionSyntax
{
public:
    AnnaSimpleNameSyntax(gcnToken tok) : VARIABLE_IDENTIFIER(tok) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnToken VARIABLE_IDENTIFIER;
};

class AnnaLiteralSyntax : public AnnaPrimaryExpressionSyntax
{
public:
    AnnaLiteralSyntax(gcnLiteralToken tok) : literal(tok) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    // STRING | REAL | INTEGER | BOOLEAN
    gcnLiteralToken literal;
};

class AnnaParenthesizedExpressionSyntax : public AnnaPrimaryExpressionSyntax
{
public:
    AnnaParenthesizedExpressionSyntax(gcnToken lpa, gcnExpression exp, gcnToken rpa) :
        OPEN_PAREN(lpa), expression(exp), CLOSE_PAREN(rpa) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnToken OPEN_PAREN;
    gcnExpression expression;
    gcnToken CLOSE_PAREN;
};

class AnnaBinaryOperatorSyntax : public AnnaSyntax
{
public:
    AnnaBinaryOperatorSyntax(gcnToken op) : binOp(op) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnToken binOp;
};

class AnnaArgumentListSyntax : public AnnaSyntax
{
public:
    AnnaArgumentListSyntax(const AnnaSeperatedList<gcnExpression> &list) :
        argumentList(list) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    AnnaSeperatedList<gcnExpression> argumentList;
};

///////////////
// Statement //
///////////////
class AnnaStatementSyntax : public AnnaSyntax
{
public:
protected:
    AnnaStatementSyntax() {}
};

class AnnaVariableDeclarationStatementSyntax : public AnnaStatementSyntax
{
public:
    AnnaVariableDeclarationStatementSyntax(gcnToken var, gcnIdentifierToken id, gcnEOS eos) :
        VAR(var), VARIABLE_IDENTIFIER(id), EOS(eos), hasAssignment(false) {}

    AnnaVariableDeclarationStatementSyntax(gcnToken var, gcnIdentifierToken id,
                                           gcnToken eq, gcnPrimaryExpression pe,
                                           gcnEOS eos) :
        VAR(var), VARIABLE_IDENTIFIER(id), EOS(eos),
        EQ_opt(eq), primaryExpression_opt(pe), hasAssignment(true) {}

    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnToken VAR;
    gcnIdentifierToken VARIABLE_IDENTIFIER;
    gcnEOS EOS;

    gcnToken EQ_opt;
    gcnPrimaryExpression primaryExpression_opt;
    bool hasAssignment;
};

class AnnaEmbeddedStatementSyntax : public AnnaStatementSyntax
{
public:
protected:
    AnnaEmbeddedStatementSyntax() {}
};

class AnnaReturnStatementSyntax : public AnnaEmbeddedStatementSyntax
{
public:
    AnnaReturnStatementSyntax(gcnToken ret, gcnExpression expr, gcnEOS e) :
        RETURN(ret), expression(expr), eos(e), hasExpr(true) {}
    AnnaReturnStatementSyntax(gcnToken ret, gcnEOS e) :
        RETURN(ret), eos(e), hasExpr(false) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnToken RETURN;
    gcnExpression expression;
    gcnEOS eos;
    bool hasExpr;
};


class AnnaBlockSyntax : public AnnaEmbeddedStatementSyntax
{
public:
    AnnaBlockSyntax(gcnToken openBra, const std::vector<gcnStatement> &stmts, gcnToken closeBra) :
        OPEN_BRACE(openBra), statements(stmts), CLOSE_BRACE(closeBra) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnToken OPEN_BRACE;
    std::vector<gcnStatement> statements;
    gcnToken CLOSE_BRACE;
};

class AnnaEmptyStatementSyntax : public AnnaEmbeddedStatementSyntax
{
public:
    AnnaEmptyStatementSyntax() : has_eos(false) {}
    AnnaEmptyStatementSyntax(gcnEOS eos) : eos_opt(eos), has_eos(true) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnEOS eos_opt;
    bool has_eos;
};

class AnnaSelectionStatementSyntax : public AnnaEmbeddedStatementSyntax
{
public:
protected:
    AnnaSelectionStatementSyntax() {}
};

class AnnaIfStatementSyntax : public AnnaSelectionStatementSyntax
{
public:
    AnnaIfStatementSyntax(gcnToken _if, gcnToken openP, gcnExpression expr,
                          gcnToken closeP, gcnEmbeddedStatement stat) :
        IF(_if), OPEN_PAREN(openP), condition(expr), CLOSE_PAREN(closeP),
        embeddedStatement(stat), hasElse(false) {}
    AnnaIfStatementSyntax(gcnToken _if, gcnToken openP, gcnExpression expr,
                          gcnToken closeP, gcnEmbeddedStatement stat,
                          gcnToken _else, gcnEmbeddedStatement elseStat) :
        IF(_if), OPEN_PAREN(openP), condition(expr), CLOSE_PAREN(closeP),
        embeddedStatement(stat), ELSE_opt(_else), elseStatement_opt(elseStat),
        hasElse(true) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnToken IF;
    gcnToken OPEN_PAREN;
    gcnExpression condition;
    gcnToken CLOSE_PAREN;
    gcnEmbeddedStatement embeddedStatement;

    gcnToken ELSE_opt;
    gcnEmbeddedStatement elseStatement_opt;
    bool hasElse;
};

class AnnaExpressionStatementSyntax : public AnnaEmbeddedStatementSyntax
{
public:
    AnnaExpressionStatementSyntax(gcnStatementExpression expr, gcnEOS e) :
        statementExpression(expr), eos(e) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnStatementExpression statementExpression;
    gcnEOS eos;
};

class AnnaIterationStatementSyntax : public AnnaEmbeddedStatementSyntax
{
public:
protected:
    AnnaIterationStatementSyntax() {}
};

class AnnaWhileStatementSyntax : public AnnaIterationStatementSyntax
{
public:
    AnnaWhileStatementSyntax(gcnToken whi, gcnToken openPar, gcnExpression cond,
                             gcnToken closePar, gcnEmbeddedStatement body) :
        WHILE(whi), OPEN_PAREN(openPar), condition(cond), CLOSE_PAREN(closePar),
        while_body(body) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnToken WHILE;
    gcnToken OPEN_PAREN;
    gcnExpression condition;
    gcnToken CLOSE_PAREN;
    gcnEmbeddedStatement while_body;
};

class AnnaStatementExpressionSyntax : public AnnaSyntax
{
public:
protected:
    AnnaStatementExpressionSyntax() {}
};

class AnnaInvocationExpressionSyntax : public AnnaPrimaryExpressionSyntax, virtual public AnnaStatementExpressionSyntax
{
public:
    AnnaInvocationExpressionSyntax(gcnFunctionIdentifier id, gcnToken opPar,
                                   gcnArgumentList args, gcnToken clPar) :
        functionIdentifier(id), OPEN_PAREN_opt(opPar), argumentList(args),
        CLOSE_PAREN_opt(clPar), hasOptionalPar(true), hasArgs(true) {}

    AnnaInvocationExpressionSyntax(gcnFunctionIdentifier id, gcnToken opPar,
                                   gcnToken clPar) :
        functionIdentifier(id), OPEN_PAREN_opt(opPar), CLOSE_PAREN_opt(clPar),
        hasOptionalPar(true), hasArgs(false) {}

    AnnaInvocationExpressionSyntax(gcnFunctionIdentifier id, gcnArgumentList args) :
        functionIdentifier(id), argumentList(args), hasOptionalPar(false),
        hasArgs(true) {}

    AnnaInvocationExpressionSyntax(gcnFunctionIdentifier id) :
        functionIdentifier(id), hasOptionalPar(false), hasArgs(false) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnFunctionIdentifier functionIdentifier;

    gcnToken OPEN_PAREN_opt;
    gcnArgumentList argumentList;
    gcnToken CLOSE_PAREN_opt;

    bool hasOptionalPar;
    bool hasArgs;
};


class AnnaAssignmentSyntax : public AnnaStatementExpressionSyntax, virtual public AnnaExpressionSyntax
{
public:
    AnnaAssignmentSyntax(gcnSimpleName l, gcnToken eq, gcnExpression r) :
        left(l), EQ(eq), right(r) {}
    void Accept(AnnaSyntaxVisitor &visitor);

//protected:
    gcnSimpleName left;
    gcnToken EQ;
    gcnExpression right;
};







#endif // ANNASYNTAX_H
