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

#include "lex_helper.h"
#include "parser.h"

AnnaParser::AnnaParser(FILE *in, const std::string &fileName)
{
    lexer_init(in, fileName);
    _filename = fileName;
    errorStreams.push(std::make_shared<std::stringstream>());

}

AnnaParser::AnnaParser(char *text, size_t len, const std::string &fileName)
{
    lexer_init(text, len, fileName);
    _filename = fileName;
    errorStreams.push(std::make_shared<std::stringstream>());
}

AnnaParser::~AnnaParser()
{
    lexer_finalize();
}

gcnCompilationUnit AnnaParser::parse()
{
    lexall();
    return parseCompilationUnit();
}

bool AnnaParser::lexall()
{
    gcnToken token = tokenize();
    while (token->token() > 0) {
        tokens.push_back(token);
        token = tokenize();
    }
    tokens.push_back(std::make_shared<AnnaToken>(END, std::make_shared<std::string>("EOF"), 0, 0, 0));
    currentToken = tokens.front();

    if (token->token() == ERROR) {
        return false;
    }

    return true;
}

gcnEOS AnnaParser::parseEOS()
{
    pushParserStatus();
    std::vector<gcnToken> eos;

    gcnToken tok = eatToken(T, "end of statement (i.e. `;' or `\\n')", __func__, true);
    if (tok)
        eos.push_back(tok);
    else {
        revertParserStatus();
        return gcnEOS();
    }

    while (peekToken(0, true)->token() == T){
        tok = eatToken(true);
        eos.push_back(tok);
    }

    popParserStatus();
    return std::make_shared<AnnaEOSSyntax>(eos);
}

gcnCompilationUnit AnnaParser::parseCompilationUnit()
{
    pushParserStatus();
    std::vector<gcnImportDirective> imports;
    std::vector<gcnVariableDeclarationStatement> variableDeclarations;
    std::vector<gcnFunctionDefinition> functionDefinitions;

    while (peekToken()->token() == IMPORT || peekToken()->token() == VAR || peekToken()->token() == DEF) {
        if (peekToken()->token() == IMPORT) {
            gcnImportDirective import = parseImportDirective();
            if (import) {
                imports.push_back(import);
                continue;
            }
        }

        if (peekToken()->token() == VAR) {
            gcnVariableDeclarationStatement variableDeclaration = parseVariableDeclarationStatement();
            if (variableDeclaration) {
                variableDeclarations.push_back(variableDeclaration);
                continue;
            }
        }

        if (peekToken()->token() == DEF) {
            gcnFunctionDefinition functionDefinition = parseFunctionDefinition();
            if (functionDefinition) {
                functionDefinitions.push_back(functionDefinition);
                continue;
            }
        }
    }

    if (peekToken()->token() == END && (!imports.empty() || !variableDeclarations.empty() || !functionDefinitions.empty())) {
        popParserStatus();
        return std::make_shared<AnnaCompilationUnitSyntax>(imports, variableDeclarations, functionDefinitions);
    } else {
        // TODO: add error output here: expected declaration ... EOF here but got ...
        revertParserStatus();
        return gcnCompilationUnit();
    }
}

gcnImportDirective AnnaParser::parseImportDirective()
{
    assert(peekToken()->token() == IMPORT);

    pushParserStatus();
    gcnToken import;
    gcnIdentifierToken identifier;
    gcnEOS eos;

    import = eatToken(IMPORT, "`import'", __func__);
    if (!import) goto not_import_directive;

    identifier = std::static_pointer_cast<IdentifierToken>(eatToken(IDENTIFIER, "identifier", __func__));
    if (!identifier) goto not_import_directive;

    eos = parseEOS();
    if (!eos) goto not_import_directive;

    popParserStatus();
    return std::make_shared<AnnaImportDirectiveSyntax>(import, identifier, eos);

not_import_directive:
    revertParserStatus();
    return gcnImportDirective();
}

gcnFunctionIdentifier AnnaParser::parseFunctionIdentifier()
{
    pushParserStatus();

    switch (peekToken(0)->token()) {
        case USER_FUNCTION_IDENTIFIER:
        case IDENTIFIER:
            popParserStatus();
            return std::make_shared<AnnaFunctionIdentifierSyntax>
                    (std::static_pointer_cast<IdentifierToken>(eatToken()));
        default:
            revertParserStatus();
            return gcnFunctionIdentifier();
    }
}

gcnExpression AnnaParser::parseExpression()
{
    pushParserStatus();
    gcnExpression expr;

    expr = parseBinaryOperationExpression();
    if (expr) { popParserStatus(); return expr; }

    expr = parseUnaryExpression();
    if (expr) { popParserStatus(); return expr; }

    expr = parseAssignment();
    if (expr) { popParserStatus(); return expr; }

    revertParserStatus();
    return gcnExpression();
}

gcnBinaryOperationExpression AnnaParser::parseBinaryOperationExpression(int min_precedence)
{
    pushParserStatus();

    gcnBinaryOperationExpression result;

    gcnExpression left;
    gcnBinaryOperator op;
    gcnExpression right;

    left = parseUnaryExpression();
    if (!left) goto not_binary_op_expr;

    while (true) {
        pushParserStatus();
        op = parseBinaryOperator();
        if (!op) {
            revertParserStatus();
            break;
        }

        int prec = getPrecedence(op->binOp->token());
        bool leftAssoc = isLeftAssociative(op->binOp->token());
        int next_min_prec = leftAssoc ? prec + 1 : prec;

        if (prec < min_precedence) {
            revertParserStatus();
            break;
        }
        popParserStatus();

        right = parseBinaryOperationExpression(next_min_prec);
        if (!right)
            right = parseUnaryExpression();
        if (!right) goto not_binary_op_expr;

        result = std::make_shared<AnnaBinaryOperationExpressionSyntax>(result ? result : left, op, right);
    }
    if (result) {
        popParserStatus();
        return result;
    } else
        goto not_binary_op_expr;

not_binary_op_expr:
    revertParserStatus();
    return gcnBinaryOperationExpression();
}

gcnUnaryExpression AnnaParser::parseUnaryExpression()
{
    pushParserStatus();
    gcnUnaryExpression expr;

    if (isPossiblePrimaryExpression()) {
        expr = parsePrimaryExpression();
        if (expr) { popParserStatus(); return expr; }
    }

    revertParserStatus();
    return gcnUnaryExpression();
}

bool AnnaParser::isPossiblePrimaryExpression()
{
    switch(peekToken()->token()){
        case STRING:
        case REAL:
        case INTEGER:
        case BOOLEAN:
        case IDENTIFIER:
        case VARIABLE_IDENTIFIER:
        case USER_FUNCTION_IDENTIFIER:
        case OPEN_PAREN:
            return true;
        default:
            return false;
    }
}

gcnPrimaryExpression AnnaParser::parsePrimaryExpression()
{
    pushParserStatus();
    gcnPrimaryExpression expr;

    expr = parseLiteral();
    if (expr) { popParserStatus(); return expr; }

    expr = parseParenthesizedExpression();
    if (expr) { popParserStatus(); return expr; }

    expr = parseInvocationExpression();
    if (expr) { popParserStatus(); return expr; }

    expr = parseSimpleName();
    if (expr) { popParserStatus(); return expr; }

    revertParserStatus();
    return gcnPrimaryExpression();
}

gcnSimpleName AnnaParser::parseSimpleName()
{
    pushParserStatus();
    gcnToken sn;
    sn = eatToken(VARIABLE_IDENTIFIER, "variable identifier /an+a/", __func__);
    if (!sn) {
        revertParserStatus();
        return gcnSimpleName();
    }

    popParserStatus();
    return std::make_shared<AnnaSimpleNameSyntax>(sn);
}

gcnLiteral AnnaParser::parseLiteral()
{
    pushParserStatus();
    switch (peekToken(0)->token()) {
        case STRING:
        case REAL:
        case INTEGER:
        case BOOLEAN:
            popParserStatus();
            return std::make_shared<AnnaLiteralSyntax>
                    (std::static_pointer_cast<LiteralToken>(eatToken()));
        default:
            revertParserStatus();
            return gcnLiteral();
    }
}

gcnParenthesizedExpression AnnaParser::parseParenthesizedExpression()
{
    pushParserStatus();
    gcnToken lPa;
    gcnExpression expr;
    gcnToken rPa;

    lPa = eatToken(OPEN_PAREN, "`('", __func__);
    if (!lPa) goto not_parenthesized_expr;

    expr = parseExpression();
    if (!expr) goto not_parenthesized_expr;

    rPa = eatToken(CLOSE_PAREN, "`('", __func__);
    if (!rPa) goto not_parenthesized_expr;

    popParserStatus();
    return std::make_shared<AnnaParenthesizedExpressionSyntax>(lPa, expr, rPa);

not_parenthesized_expr:
    revertParserStatus();
    return gcnParenthesizedExpression();
}

gcnBinaryOperator AnnaParser::parseBinaryOperator()
{
    pushParserStatus();
    switch (peekToken(0)->token()) {
        case GE:
        case LE:
        case NE:
        case EE:
        case GT:
        case LT:

        case ANDAND:
        case OROR:

        case AND:
        case OR:
        case XOR:

        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case MOD:

            popParserStatus();
            return std::make_shared<AnnaBinaryOperatorSyntax>(eatToken());
        default:
            revertParserStatus();
            return gcnBinaryOperator();
    }
}

gcnInvocationExpression AnnaParser::parseInvocationExpression()
{
    pushParserStatus();

    gcnFunctionIdentifier id;
    gcnArgumentList list;

    gcnToken optOpenP;
    gcnToken optCloseP;

    id = parseFunctionIdentifier();
    if (!id) {
        revertParserStatus();
        return gcnInvocationExpression();
    }

    if (peekToken(0)->token() == OPEN_PAREN) {
        optOpenP = eatToken();
        list = parseArgumentList();
        optCloseP = eatToken(CLOSE_PAREN, "`)'", __func__);
        if (!optOpenP || !optCloseP) {
            revertParserStatus();
            return gcnInvocationExpression();
        }

        popParserStatus();
        if (list)
            return std::make_shared<AnnaInvocationExpressionSyntax>(id, optOpenP, list, optCloseP);
        else
            return std::make_shared<AnnaInvocationExpressionSyntax>(id, optOpenP, optCloseP);

    } else {
        list = parseArgumentList();

        popParserStatus();
        if (list)
            return std::make_shared<AnnaInvocationExpressionSyntax>(id, list);
        else
            return std::make_shared<AnnaInvocationExpressionSyntax>(id);
    }
}

gcnArgumentList AnnaParser::parseArgumentList()
{
    pushParserStatus();
    AnnaSeperatedList<gcnExpression> list;

    gcnExpression expr;

    expr = parseExpression();
    if (!expr) {
        revertParserStatus();
        return gcnArgumentList();
    } else
        list.add(expr);

    while (peekToken(0)->token() == COMMA) {
        list.addSeperator(eatToken());
        expr = parseExpression();
        if (!expr) {
            revertParserStatus();
            return gcnArgumentList();
        }
        list.add(expr);
    }

    popParserStatus();
    return std::make_shared<AnnaArgumentListSyntax>(list);
}

gcnFunctionDefinition AnnaParser::parseFunctionDefinition()
{
    assert(peekToken()->token() == DEF);

    pushParserStatus();
    gcnFunctionHeader header;
    gcnFunctionBody body;
    header = parseFunctionHeader();
    if (!header) goto not_function_definition;
    body = parseFunctionBody();
    if (!body) goto not_function_definition;

    popParserStatus();
    return std::make_shared<AnnaFunctionDefinitionSyntax>(header, body);

not_function_definition:
    revertParserStatus();
    return gcnFunctionDefinition();
}

gcnFunctionHeader AnnaParser::parseFunctionHeader()
{
    assert(peekToken()->token() == DEF);

    pushParserStatus();
    gcnToken def;
    gcnIdentifierToken id;
    gcnToken openPar;
    gcnFormalParameterList parameters;
    gcnToken closePar;
    bool hasParam;
    def = eatToken(DEF, "`def'", __func__);
    if (!def) goto not_function_header;

    id = std::static_pointer_cast<IdentifierToken>
            (eatToken(USER_FUNCTION_IDENTIFIER, "user function identifier starts with @"));
    if (!id) goto not_function_header;

    openPar = eatToken(OPEN_PAREN, "`('", __func__);
    if (!openPar) goto not_function_header;

    if (peekToken(0)->token() == CLOSE_PAREN) {
        hasParam = false;
        closePar = eatToken(CLOSE_PAREN, "`)'", __func__);
        if (!closePar) goto not_function_header;
    } else {
        hasParam = true;
        parameters = parseFormalParameterList();
        if (!parameters) goto not_function_header;

        closePar = eatToken(CLOSE_PAREN, "`)'", __func__);
        if (!closePar) goto not_function_header;
    }

    popParserStatus();
    if (hasParam)
        return std::make_shared<AnnaFunctionHeaderSyntax>(def, id, openPar, parameters, closePar);
    else
        return std::make_shared<AnnaFunctionHeaderSyntax>(def, id, openPar, closePar);

not_function_header:
    revertParserStatus();
    return gcnFunctionHeader();
}

gcnFormalParameterList AnnaParser::parseFormalParameterList()
{
    pushParserStatus();
    AnnaSeperatedList<gcnFormalParameter> list;
    gcnFormalParameter param;

    param = parseFormalParameter();
    if (!param)
        goto not_formal_param_list;
    else
        list.add(param);

    while (peekToken(0)->token() == COMMA) {
        gcnToken comma = eatToken(COMMA, "`,'", __func__);
        assert(comma);
        param = parseFormalParameter();
        if (!param)
            goto not_formal_param_list;
        list.addSeperator(comma);
        list.add(param);
    }
    popParserStatus();
    return std::make_shared<AnnaFormalParameterListSyntax>(list);

not_formal_param_list:
    revertParserStatus();
    return gcnFormalParameterList();
}

gcnFormalParameter AnnaParser::parseFormalParameter()
{
    pushParserStatus();
    gcnIdentifierToken param;
    param = std::static_pointer_cast<IdentifierToken>(eatToken(VARIABLE_IDENTIFIER, "variable identifier /an+a/", __func__));
    if (!param) {
        revertParserStatus();
        return gcnFormalParameter();
    } else {
        popParserStatus();
        return std::make_shared<AnnaFormalParameterSyntax>(param);
    }
}

gcnFunctionBody AnnaParser::parseFunctionBody()
{
    pushParserStatus();
    gcnBlock block = parseBlock();
    if (!block) {
        revertParserStatus();
        return gcnFunctionBody();
    }

    popParserStatus();
    return std::make_shared<AnnaFunctionBodySyntax>(block);
}

gcnBlock AnnaParser::parseBlock()
{
    pushParserStatus();
    gcnToken openBra;
    gcnToken closeBra;
    std::vector<gcnStatement> statements;
    gcnStatement statement;

    openBra = eatToken(OPEN_BRACE, "`{'", __func__);
    if (!openBra) goto not_block;

    do {
        statement = parseStatement();
        if (statement)
            statements.push_back(statement);
    } while (statement);

    closeBra = eatToken(CLOSE_BRACE, "`}'", __func__);
    if (!closeBra) goto not_block;

    // This will allow empty block
    if (statements.empty() && !closeBra) {
        fprintf(__log_out, "Expect statements\n");
        goto not_block;
    }

    popParserStatus();
    return std::make_shared<AnnaBlockSyntax>(openBra, statements, closeBra);

not_block:
    revertParserStatus();
    return gcnBlock();
}

gcnStatement AnnaParser::parseStatement()
{
    pushParserStatus();
    gcnStatement stat;

    if (peekToken()->token() == VAR) {
        stat = parseVariableDeclarationStatement();
        if (stat) { popParserStatus(); return stat; }
    }

    stat = parseEmbeddedStatement();
    if (stat) { popParserStatus(); return stat; }

    revertParserStatus();
    return gcnStatement();
}

gcnEmbeddedStatement AnnaParser::parseEmbeddedStatement()
{
    pushParserStatus();
    gcnEmbeddedStatement stat;

    stat = parseBlock();
    if (stat) { popParserStatus(); return stat; }

    stat = parseIterationStatement();
    if (stat) { popParserStatus(); return stat; }

    stat = parseSelectionStatement();
    if (stat) { popParserStatus(); return stat; }

    stat = parseExpressionStatement();
    if (stat) { popParserStatus(); return stat; }

    stat = parseReturnStatement();
    if (stat) { popParserStatus(); return stat; }

    stat = parseEmptyStatement();
    if (stat) { popParserStatus(); return stat; }

    revertParserStatus();
    return gcnEmbeddedStatement();
}

gcnVariableDeclarationStatement AnnaParser::parseVariableDeclarationStatement()
{
    assert(peekToken()->token() == VAR);

    pushParserStatus();
    gcnToken var;
    gcnIdentifierToken varid;
    bool hasAssign = false;
    gcnToken eq;
    gcnPrimaryExpression primaryExpr;
    gcnEOS eos;

    var = eatToken(VAR, "`var", __func__);
    if (!var) goto not_var_declaration;

    varid = std::static_pointer_cast<IdentifierToken>(eatToken(VARIABLE_IDENTIFIER));
    if (!varid) goto not_var_declaration;

    if (peekToken(0)->token() == EQ) {
        hasAssign = true;

        eq = eatToken(EQ, "`='", __func__);
        if (!eq) goto not_var_declaration;

        primaryExpr = parsePrimaryExpression();
        if (!primaryExpr) goto not_var_declaration;

        eos = parseEOS();
        if (!eos) goto not_var_declaration;
    } else {
        hasAssign = false;
        eos = parseEOS();
        if (!eos) goto not_var_declaration;
    }

    if (hasAssign) {
        popParserStatus();
        return std::make_shared<AnnaVariableDeclarationStatementSyntax>
                (var, varid, eq, primaryExpr, eos);
    } else {
        popParserStatus();
        return std::make_shared<AnnaVariableDeclarationStatementSyntax>
                (var, varid, eos);
    }

not_var_declaration:
    revertParserStatus();
    return gcnVariableDeclarationStatement();
}

gcnEmptyStatement AnnaParser::parseEmptyStatement()
{
    pushParserStatus();
    gcnEOS eos = parseEOS();
    if (eos) {
        popParserStatus();
        return std::make_shared<AnnaEmptyStatementSyntax>(eos);
    } else {
        revertParserStatus();
        return gcnEmptyStatement();
    }
}

gcnExpressionStatement AnnaParser::parseExpressionStatement()
{
    pushParserStatus();
    gcnStatementExpression statExpr;
    gcnEOS eos;
    statExpr = parseStatementExpression();
    if (!statExpr) goto not_expr_statement;
    eos = parseEOS();
    if (!eos) goto not_expr_statement;

    popParserStatus();
    return std::make_shared<AnnaExpressionStatementSyntax>(statExpr, eos);

not_expr_statement:
    revertParserStatus();
    return gcnExpressionStatement();
}

gcnStatementExpression AnnaParser::parseStatementExpression()
{
    pushParserStatus();
    gcnStatementExpression expr;

    expr = parseInvocationExpression();
    if (expr) { popParserStatus(); return expr; }

    expr = parseAssignment();
    if (expr) { popParserStatus(); return expr; }

    revertParserStatus();
    return gcnStatementExpression();
}

gcnSelectionStatement AnnaParser::parseSelectionStatement()
{
    pushParserStatus();
    gcnSelectionStatement stat;

    // isPossibleIf
    if (peekToken(0)->token() == IF) {
        stat = parseIfStatement();
        if (stat) { popParserStatus(); return stat; }
    }

    revertParserStatus();
    return gcnSelectionStatement();
}

gcnIfStatement AnnaParser::parseIfStatement()
{
    pushParserStatus();
    gcnToken _if;
    gcnToken openPar;
    gcnExpression expr;
    gcnToken closePar;
    gcnEmbeddedStatement stat;

    _if = eatToken(IF, "`if", __func__);
    if (!_if) goto not_if_statement;

    openPar = eatToken(OPEN_PAREN, "`('", __func__);
    if (!openPar) goto not_if_statement;

    expr = parseExpression();
    if (!expr) goto not_if_statement;

    closePar = eatToken(CLOSE_PAREN, "`)'", __func__);
    if (!closePar) goto not_if_statement;

    stat = parseEmbeddedStatement();
    if (!stat) goto not_if_statement;

    if (peekToken(0)->token() == ELSE) {
        gcnToken _else;
        gcnEmbeddedStatement elseStat;
        _else = eatToken(ELSE, "`else'", __func__);
        if (!_else) goto not_if_statement;

        elseStat = parseEmbeddedStatement();
        if (!elseStat) goto not_if_statement;

        popParserStatus();
        return std::make_shared<AnnaIfStatementSyntax>
                (_if, openPar, expr, closePar, stat, _else, elseStat);
    }

    popParserStatus();
    return std::make_shared<AnnaIfStatementSyntax>(_if, openPar, expr, closePar, stat);

not_if_statement:
    revertParserStatus();
    return gcnIfStatement();
}

gcnIterationStatement AnnaParser::parseIterationStatement()
{
    pushParserStatus();
    gcnIterationStatement stat;

    // isPossibleWhile
    stat = parseWhileStatement();
    if (stat) { popParserStatus(); return stat; }

    popParserStatus();
    return gcnIterationStatement();
}

gcnWhileStatement AnnaParser::parseWhileStatement()
{
    pushParserStatus();

    gcnToken _while;
    gcnToken openPar;
    gcnExpression expr;
    gcnToken closePar;
    gcnEmbeddedStatement stats;

    _while = eatToken(WHILE, "`while'", __func__);
    if (!_while) goto not_while_statement;

    openPar = eatToken(OPEN_PAREN, "`('", __func__);
    if (!openPar) goto not_while_statement;

    expr = parseExpression();
    if (!expr) goto not_while_statement;

    closePar = eatToken(CLOSE_PAREN, "`)'", __func__);
    if (!closePar) goto not_while_statement;

    stats = parseEmbeddedStatement();
    if (!stats) goto not_while_statement;

    popParserStatus();
    return std::make_shared<AnnaWhileStatementSyntax>(_while, openPar, expr, closePar, stats);

not_while_statement:
    revertParserStatus();
    return gcnWhileStatement();
}

gcnAssignment AnnaParser::parseAssignment()
{
    pushParserStatus();

    gcnSimpleName left;
    gcnToken eq;
    gcnExpression right;

    left = parseSimpleName();
    if (!left) goto not_assignment;

    eq = eatToken(EQ, "``=''", __func__);
    if (!eq) goto not_assignment;

    right = parseExpression();
    if (!right) goto not_assignment;

    popParserStatus();
    return std::make_shared<AnnaAssignmentSyntax>(left, eq, right);

not_assignment:
    revertParserStatus();
    return gcnAssignment();
}

gcnReturnStatement AnnaParser::parseReturnStatement()
{
    pushParserStatus();
    gcnToken ret;
    gcnExpression expr;
    gcnEOS eos;

    ret = eatToken(RETURN, "`return'", __func__);
    if (!ret) goto not_return_statement;

    if (peekToken(0, true)->token() == T) {
        eos = parseEOS();
        if (!eos) goto not_return_statement;

        return std::make_shared<AnnaReturnStatementSyntax>(ret, eos);
    } else {
        expr = parseExpression();
        if (!expr) goto not_return_statement;
        eos = parseEOS();
        if (!eos) goto not_return_statement;

        return std::make_shared<AnnaReturnStatementSyntax>(ret, expr, eos);
    }

not_return_statement:
    revertParserStatus();
    return gcnReturnStatement();
}

int AnnaParser::getPrecedence(Tokens op)
{
    switch (op) {
        case OROR:
            return 1;

        case ANDAND:
            return 2;

        case OR:
            return 3;

        case XOR:
            return 4;

        case AND:
            return 5;

        case EE:
        case NE:
            return 6;

        case GE:
        case LE:
        case GT:
        case LT:
            return 7;

        case ADD:
        case SUB:
            return 8;

        case MUL:
        case DIV:
        case MOD:
            return 9;

        default:
            throw;
    }
}

bool AnnaParser::isLeftAssociative(Tokens op)
{
    switch (op) {
        case OROR:
        case ANDAND:
        case OR:
        case XOR:
        case AND:
        case EE:
        case NE:
        case GE:
        case LE:
        case GT:
        case LT:
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case MOD:
            return true;
        default:
            throw;
    }
}


gcnToken AnnaParser::eatToken(bool dontIgnoreNewlineT)
{
    if (!dontIgnoreNewlineT) {
        for (size_t i = currentTokenIdx; i < tokens.size(); ++i) {
            if (!isNewlineT(tokens[i])) {
                currentTokenIdx = i + 1;
                currentToken = tokens[currentTokenIdx];
                return tokens[i];
            }
        }
        // Out of bound
        currentToken = tokens[tokens.size() - 1];
        currentTokenIdx = tokens.size() - 1;
        return gcnToken();
    } else {
        if (++currentTokenIdx < tokens.size()) {
            gcnToken tok = currentToken;
            currentToken = tokens[currentTokenIdx];
            return tok;
        } else {
            currentTokenIdx = tokens.size() - 1;
            currentToken = tokens[currentTokenIdx];
            return currentToken;
        }
    }
}

gcnToken AnnaParser::eatToken(Tokens kind, const char *expected, const char *caller, bool dontIgnoreNewlineT)
{
    pushParserStatus();

    gcnToken tok = eatToken(dontIgnoreNewlineT);

    int row = tok->row();
    int col = tok->col();
    int width = tok->width();
    if (tok->token() != kind) {
        revertParserStatus();

        log_print_pos(row, col, _filename, currentErrorStream());
        if (expected) {
            currentErrorStream() << "Invalid token `" << tok->text()->c_str()
                                 << "', expected " << expected << " in " << caller << "\n";
        } else {
            currentErrorStream() << "Invalid token `" << tok->text()->c_str() << "'\n";
        }
        log_print_row(row, currentErrorStream());
        log_print_indicators(col, width, currentErrorStream());
        return gcnToken();
    }

    popParserStatus();
    return tok;
}

gcnToken AnnaParser::peekToken(int ahead, bool dontIgnoreNewlineT)
{
    for (unsigned int i = ahead + currentTokenIdx; i < tokens.size(); ++i) {
        if (dontIgnoreNewlineT)
            return tokens.at(i);

        if (!isNewlineT(tokens[i]))
            return tokens[i];
    }
    return gcnToken();
}

void AnnaParser::revertToken(unsigned int index)
{
    assert(index < tokens.size());

    if (index < tokens.size()) {
        currentToken = tokens[index];
        currentTokenIdx = index;
    }
}

bool AnnaParser::isNewlineT(const gcnToken &token)
{
    if (token->token() == T && !(token->text()->compare("\n")))
        return true;
    else
        return false;
}



