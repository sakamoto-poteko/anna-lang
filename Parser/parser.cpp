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
}

AnnaParser::AnnaParser(char *text, size_t len, const std::string &fileName)
{
    lexer_init(text, len, fileName);
    _filename = fileName;
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
    gcnToken token = lex();
    while (token->token() > 0) {
        tokens.push_back(token);
        token = lex();
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
    pushTokenStatus();
    std::vector<gcnToken> eos;

    gcnToken tok = eatToken(T, "end of statement (i.e. `;' or `\\n')", true);
    if (tok)
        eos.push_back(tok);
    else {
        revertTokenStatus();
        return gcnEOS();
    }

    while (peekToken(0, true)->token() == T){
        tok = eatToken(true);
        eos.push_back(tok);
    }

    popTokenStatus();
    return std::make_shared<AnnaEOSSyntax>(eos);
}

gcnCompilationUnit AnnaParser::parseCompilationUnit()
{
    pushTokenStatus();
    std::vector<gcnImportDirective> imports;
    std::vector<gcnVariableDeclarationStatement> variableDeclarations;
    std::vector<gcnFunctionDefinition> functionDefinitions;

    // Import Directives
    // isPossibleImport
    while (1) {
        gcnImportDirective import = parseImportDirective();
        if (import)
            imports.push_back(import);
        else
            break;
    }

    // isPossibleVariableDeclaration
    while (1) {
        gcnVariableDeclarationStatement variableDeclaration = parseVariableDeclarationStatement();
        if (variableDeclaration)
            variableDeclarations.push_back(variableDeclaration);
        else
            break;
    }

    // isPossibleFunctionDefinition
    while (1) {
        gcnFunctionDefinition functionDefinition = parseFunctionDefinition();
        if (functionDefinition)
            functionDefinitions.push_back(functionDefinition);
        else
            break;
    }

    popTokenStatus();
    return std::make_shared<AnnaCompilationUnitSyntax>(imports, variableDeclarations, functionDefinitions);
}

gcnImportDirective AnnaParser::parseImportDirective()
{
    pushTokenStatus();
    gcnToken import;
    gcnIdentifierToken identifier;
    gcnEOS eos;

    import = eatToken(IMPORT, "`import'");
    if (!import) goto not_import_directive;

    identifier = std::static_pointer_cast<IdentifierToken>(eatToken(IDENTIFIER, "identifier"));
    if (!identifier) goto not_import_directive;

    eos = parseEOS();
    if (!eos) goto not_import_directive;

    popTokenStatus();
    return std::make_shared<AnnaImportDirectiveSyntax>(import, identifier, eos);

not_import_directive:
    revertTokenStatus();
    return gcnImportDirective();
}

gcnFunctionIdentifier AnnaParser::parseFunctionIdentifier()
{
    pushTokenStatus();

    switch (peekToken(0)->token()) {
        case USER_FUNCTION_IDENTIFIER:
        case IDENTIFIER:
            popTokenStatus();
            return std::make_shared<AnnaFunctionIdentifierSyntax>
                    (std::static_pointer_cast<IdentifierToken>(eatToken()));
        default:
            revertTokenStatus();
            return gcnFunctionIdentifier();
    }
}

gcnExpression AnnaParser::parseExpression()
{
    pushTokenStatus();
    gcnExpression expr;

    expr = parseBinaryOperationExpression();
    if (expr) { popTokenStatus(); return expr; }

    expr = parseUnaryExpression();
    if (expr) { popTokenStatus(); return expr; }

    expr = parseAssignment();
    if (expr) { popTokenStatus(); return expr; }

    revertTokenStatus();
    return gcnExpression();
}

gcnBinaryOperationExpression AnnaParser::parseBinaryOperationExpression()
{
    pushTokenStatus();

    gcnUnaryExpression left;
    gcnBinaryOperator op;
    gcnExpression right;

    left = parseUnaryExpression();
    if (!left) goto not_binary_op_expr;

    op = parseBinaryOperator();
    if (!op)  goto not_binary_op_expr;

    right = parseExpression();
    if (!right) goto not_binary_op_expr;

    popTokenStatus();
    return std::make_shared<AnnaBinaryOperationExpressionSyntax>(left, op, right);

not_binary_op_expr:
    revertTokenStatus();
    return gcnBinaryOperationExpression();
}

gcnUnaryExpression AnnaParser::parseUnaryExpression()
{
    pushTokenStatus();
    gcnUnaryExpression expr;

    expr = parsePrimaryExpression();
    if (expr) { popTokenStatus(); return expr; }

    revertTokenStatus();
    return gcnUnaryExpression();
}

gcnPrimaryExpression AnnaParser::parsePrimaryExpression()
{
    pushTokenStatus();
    gcnPrimaryExpression expr;

    expr = parseLiteral();
    if (expr) { popTokenStatus(); return expr; }

    expr = parseParenthesizedExpression();
    if (expr) { popTokenStatus(); return expr; }

    expr = parseInvocationExpression();
    if (expr) { popTokenStatus(); return expr; }

    expr = parseSimpleName();
    if (expr) { popTokenStatus(); return expr; }

    revertTokenStatus();
    return gcnPrimaryExpression();
}

gcnSimpleName AnnaParser::parseSimpleName()
{
    pushTokenStatus();
    gcnToken sn;
    sn = eatToken(VARIABLE_IDENTIFIER, "variable identifier /an+a/");
    if (!sn) {
        revertTokenStatus();
        return gcnSimpleName();
    }

    popTokenStatus();
    return std::make_shared<AnnaSimpleNameSyntax>(sn);
}

gcnLiteral AnnaParser::parseLiteral()
{
    pushTokenStatus();
    switch (peekToken(0)->token()) {
        case STRING:
        case REAL:
        case INTEGER:
        case BOOLEAN:
            popTokenStatus();
            return std::make_shared<AnnaLiteralSyntax>
                    (std::static_pointer_cast<LiteralToken>(eatToken()));
        default:
            revertTokenStatus();
            return gcnLiteral();
    }
}

gcnParenthesizedExpression AnnaParser::parseParenthesizedExpression()
{
    pushTokenStatus();
    gcnToken lPa;
    gcnExpression expr;
    gcnToken rPa;

    lPa = eatToken(OPEN_PAREN, "`('");
    if (!lPa) goto not_parenthesized_expr;

    expr = parseExpression();
    if (!expr) goto not_parenthesized_expr;

    rPa = eatToken(CLOSE_PAREN, "`('");
    if (!rPa) goto not_parenthesized_expr;

    popTokenStatus();
    return std::make_shared<AnnaParenthesizedExpressionSyntax>(lPa, expr, rPa);

not_parenthesized_expr:
    revertTokenStatus();
    return gcnParenthesizedExpression();
}

gcnBinaryOperator AnnaParser::parseBinaryOperator()
{
    pushTokenStatus();
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

            popTokenStatus();
            return std::make_shared<AnnaBinaryOperatorSyntax>(eatToken());
        default:
            revertTokenStatus();
            return gcnBinaryOperator();
    }
}

gcnInvocationExpression AnnaParser::parseInvocationExpression()
{
    pushTokenStatus();

    gcnFunctionIdentifier id;
    gcnArgumentList list;

    gcnToken optOpenP;
    gcnToken optCloseP;

    id = parseFunctionIdentifier();
    if (!id) {
        revertTokenStatus();
        return gcnInvocationExpression();
    }

    if (peekToken(0)->token() == OPEN_PAREN) {
        optOpenP = eatToken();
        list = parseArgumentList();
        optCloseP = eatToken(CLOSE_PAREN, "`)'");
        if (!!optOpenP || !optCloseP) {
            revertTokenStatus();
            return gcnInvocationExpression();
        }

        popTokenStatus();
        if (list)
            return std::make_shared<AnnaInvocationExpressionSyntax>(id, optOpenP, list, optCloseP);
        else
            return std::make_shared<AnnaInvocationExpressionSyntax>(id, optOpenP, optCloseP);

    } else {
        list = parseArgumentList();

        popTokenStatus();
        if (list)
            return std::make_shared<AnnaInvocationExpressionSyntax>(id, list);
        else
            return std::make_shared<AnnaInvocationExpressionSyntax>(id);
    }
}

gcnArgumentList AnnaParser::parseArgumentList()
{
    pushTokenStatus();
    AnnaSeperatedList<gcnExpression> list;

    gcnExpression expr;

    expr = parseExpression();
    if (!expr) {
        revertTokenStatus();
        return gcnArgumentList();
    } else
        list.add(expr);

    while (peekToken(0)->token() == COMMA) {
        list.addSeperator(eatToken());
        expr = parseExpression();
        if (!expr) {
            revertTokenStatus();
            return gcnArgumentList();
        }
    }

    popTokenStatus();
    return std::make_shared<AnnaArgumentListSyntax>(list);
}

gcnFunctionDefinition AnnaParser::parseFunctionDefinition()
{
    pushTokenStatus();
    gcnFunctionHeader header;
    gcnFunctionBody body;
    header = parseFunctionHeader();
    if (!header) goto not_function_definition;
    body = parseFunctionBody();
    if (!body)goto not_function_definition;

    popTokenStatus();
    return std::make_shared<AnnaFunctionDefinitionSyntax>(header, body);

not_function_definition:
    revertTokenStatus();
    return gcnFunctionDefinition();
}

gcnFunctionHeader AnnaParser::parseFunctionHeader()
{
    pushTokenStatus();
    gcnToken def;
    gcnIdentifierToken id;
    gcnToken openPar;
    gcnFormalParameterList parameters;
    gcnToken closePar;
    bool hasParam;
    def = eatToken(DEF, "`def'");
    if (!def) goto not_function_header;

    id = std::static_pointer_cast<IdentifierToken>
            (eatToken(USER_FUNCTION_IDENTIFIER, "user function identifier starts with @"));
    if (!id) goto not_function_header;

    openPar = eatToken(OPEN_PAREN, "`('");
    if (!openPar) goto not_function_header;

    if (peekToken(0)->token() == CLOSE_PAREN) {
        hasParam = false;
        closePar = eatToken(CLOSE_PAREN, "`)'");
        if (!closePar) goto not_function_header;
    } else {
        hasParam = true;
        parameters = parseFormalParameterList();
        if (!parameters) goto not_function_header;

        closePar = eatToken(CLOSE_PAREN, "`)'");
        if (!closePar) goto not_function_header;
    }

    popTokenStatus();
    if (hasParam)
        return std::make_shared<AnnaFunctionHeaderSyntax>(def, id, openPar, parameters, closePar);
    else
        return std::make_shared<AnnaFunctionHeaderSyntax>(def, id, openPar, closePar);

not_function_header:
    revertTokenStatus();
    return gcnFunctionHeader();
}

gcnFormalParameterList AnnaParser::parseFormalParameterList()
{
    pushTokenStatus();
    AnnaSeperatedList<gcnFormalParameter> list;
    gcnFormalParameter param;

    param = parseFormalParameter();
    if (!param)
        goto not_formal_param_list;
    else
        list.add(param);

    while (peekToken(0)->token() == COMMA) {
        gcnToken comma = eatToken(COMMA, "`,'");
        assert(comma);
        param = parseFormalParameter();
        if (!param)
            goto not_formal_param_list;
        list.addSeperator(comma);
        list.add(param);
    }
    popTokenStatus();
    return std::make_shared<AnnaFormalParameterListSyntax>(list);

not_formal_param_list:
    revertTokenStatus();
    return gcnFormalParameterList();
}

gcnFormalParameter AnnaParser::parseFormalParameter()
{
    pushTokenStatus();
    gcnIdentifierToken param;
    param = std::static_pointer_cast<IdentifierToken>(eatToken(VARIABLE_IDENTIFIER, "variable identifier /an+a/"));
    if (!param) {
        revertTokenStatus();
        return gcnFormalParameter();
    } else {
        popTokenStatus();
        return std::make_shared<AnnaFormalParameterSyntax>(param);
    }
}

gcnFunctionBody AnnaParser::parseFunctionBody()
{
    pushTokenStatus();
    gcnBlock block = parseBlock();
    if (!block) {
        revertTokenStatus();
        return gcnFunctionBody();
    }

    popTokenStatus();
    return std::make_shared<AnnaFunctionBodySyntax>(block);
}

gcnBlock AnnaParser::parseBlock()
{
    pushTokenStatus();
    gcnToken openBra;
    gcnToken closeBra;
    std::vector<gcnStatement> statements;
    gcnStatement statement;

    openBra = eatToken(OPEN_BRACE, "`{'");
    if (!openBra) goto not_block;

    do {
        statement = parseStatement();
        if (statement)
            statements.push_back(statement);
    } while (statement);

    closeBra = eatToken(CLOSE_BRACE, "`}'");
    if (!closeBra) goto not_block;

    // This will allow empty block
    if (statements.empty() && !closeBra) {
        fprintf(__log_out, "Expect statements\n");
        goto not_block;
    }

    popTokenStatus();
    return std::make_shared<AnnaBlockSyntax>(openBra, statements, closeBra);

not_block:
    revertTokenStatus();
    return gcnBlock();
}

gcnStatement AnnaParser::parseStatement()
{
    pushTokenStatus();
    gcnStatement stat;

    stat = parseVariableDeclarationStatement();
    if (stat) { popTokenStatus(); return stat; }

    stat = parseEmbeddedStatement();
    if (stat) { popTokenStatus(); return stat; }

    revertTokenStatus();
    return gcnStatement();
}

gcnEmbeddedStatement AnnaParser::parseEmbeddedStatement()
{
    pushTokenStatus();
    gcnEmbeddedStatement stat;

    stat = parseBlock();
    if (stat) { popTokenStatus(); return stat; }

    stat = parseIterationStatement();
    if (stat) { popTokenStatus(); return stat; }

    stat = parseSelectionStatement();
    if (stat) { popTokenStatus(); return stat; }

    stat = parseExpressionStatement();
    if (stat) { popTokenStatus(); return stat; }

    stat = parseReturnStatement();
    if (stat) { popTokenStatus(); return stat; }

    stat = parseEmptyStatement();
    if (stat) { popTokenStatus(); return stat; }

    revertTokenStatus();
    return gcnEmbeddedStatement();
}

gcnVariableDeclarationStatement AnnaParser::parseVariableDeclarationStatement()
{
    pushTokenStatus();
    gcnToken var;
    gcnIdentifierToken varid;
    bool hasAssign = false;
    gcnToken eq;
    gcnPrimaryExpression primaryExpr;
    gcnEOS eos;

    var = eatToken(VAR, "`var");
    if (!var) goto not_var_declaration;

    varid = std::static_pointer_cast<IdentifierToken>(eatToken(VARIABLE_IDENTIFIER));
    if (!varid) goto not_var_declaration;

    if (peekToken(0)->token() == EQ) {
        hasAssign = true;

        eq = eatToken(EQ, "`='");
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
        popTokenStatus();
        return std::make_shared<AnnaVariableDeclarationStatementSyntax>
                (var, varid, eq, primaryExpr, eos);
    } else {
        popTokenStatus();
        return std::make_shared<AnnaVariableDeclarationStatementSyntax>
                (var, varid, eos);
    }

not_var_declaration:
    revertTokenStatus();
    return gcnVariableDeclarationStatement();
}

gcnEmptyStatement AnnaParser::parseEmptyStatement()
{
    pushTokenStatus();
    gcnEOS eos = parseEOS();
    if (eos) {
        popTokenStatus();
        return std::make_shared<AnnaEmptyStatementSyntax>(eos);
    } else {
        revertTokenStatus();
        return gcnEmptyStatement();
    }
}

gcnExpressionStatement AnnaParser::parseExpressionStatement()
{
    pushTokenStatus();
    gcnStatementExpression statExpr;
    gcnEOS eos;
    statExpr = parseStatementExpression();
    if (!statExpr) goto not_expr_statement;
    eos = parseEOS();
    if (!eos) goto not_expr_statement;

    popTokenStatus();
    return std::make_shared<AnnaExpressionStatementSyntax>(statExpr, eos);

not_expr_statement:
    revertTokenStatus();
    return gcnExpressionStatement();
}

gcnStatementExpression AnnaParser::parseStatementExpression()
{
    pushTokenStatus();
    gcnStatementExpression expr;

    expr = parseInvocationExpression();
    if (expr) { popTokenStatus(); return expr; }

    expr = parseAssignment();
    if (expr) { popTokenStatus(); return expr; }

    revertTokenStatus();
    return gcnStatementExpression();
}

gcnSelectionStatement AnnaParser::parseSelectionStatement()
{
    pushTokenStatus();
    gcnSelectionStatement stat;

    // isPossibleIf
    if (peekToken(0)->token() == IF) {
        stat = parseIfStatement();
        if (stat) { popTokenStatus(); return stat; }
    }

    revertTokenStatus();
    return gcnSelectionStatement();
}

gcnIfStatement AnnaParser::parseIfStatement()
{
    pushTokenStatus();
    gcnToken _if;
    gcnToken openPar;
    gcnExpression expr;
    gcnToken closePar;
    gcnEmbeddedStatement stat;

    _if = eatToken(IF, "`if");
    if (!_if) goto not_if_statement;

    openPar = eatToken(OPEN_PAREN, "`('");
    if (!openPar) goto not_if_statement;

    expr = parseExpression();
    if (!expr) goto not_if_statement;

    closePar = eatToken(CLOSE_PAREN, "`)'");
    if (!closePar) goto not_if_statement;

    stat = parseEmbeddedStatement();
    if (!stat) goto not_if_statement;

    if (peekToken(0)->token() == ELSE) {
        gcnToken _else;
        gcnEmbeddedStatement elseStat;
        _else = eatToken(ELSE, "`else'");
        if (!_else) goto not_if_statement;

        elseStat = parseEmbeddedStatement();
        if (!elseStat) goto not_if_statement;

        popTokenStatus();
        return std::make_shared<AnnaIfStatementSyntax>
                (_if, openPar, expr, closePar, stat, _else, elseStat);
    }

    popTokenStatus();
    return std::make_shared<AnnaIfStatementSyntax>(_if, openPar, expr, closePar, stat);

not_if_statement:
    revertTokenStatus();
    return gcnIfStatement();
}

gcnIterationStatement AnnaParser::parseIterationStatement()
{
    pushTokenStatus();
    gcnIterationStatement stat;

    // isPossibleWhile
    stat = parseWhileStatement();
    if (stat) { popTokenStatus(); return stat; }

    popTokenStatus();
    return gcnIterationStatement();
}

gcnWhileStatement AnnaParser::parseWhileStatement()
{
    pushTokenStatus();

    gcnToken _while;
    gcnToken openPar;
    gcnExpression expr;
    gcnToken closePar;
    gcnEmbeddedStatement stats;

    _while = eatToken(WHILE, "`while'");
    if (!_while) goto not_while_statement;

    openPar = eatToken(OPEN_PAREN, "`('");
    if (!openPar) goto not_while_statement;

    expr = parseExpression();
    if (!expr) goto not_while_statement;

    closePar = eatToken(CLOSE_PAREN, "`)'");
    if (!closePar) goto not_while_statement;

    stats = parseEmbeddedStatement();
    if (!stats) goto not_while_statement;

    popTokenStatus();
    return std::make_shared<AnnaWhileStatementSyntax>(_while, openPar, expr, closePar, stats);

not_while_statement:
    revertTokenStatus();
    return gcnWhileStatement();
}

gcnAssignment AnnaParser::parseAssignment()
{
    pushTokenStatus();

    gcnSimpleName left;
    gcnToken eq;
    gcnExpression right;

    left = parseSimpleName();
    if (!left) goto not_assignment;

    eq = eatToken(EQ, "``=''");
    if (!eq) goto not_assignment;

    right = parseExpression();
    if (!right) goto not_assignment;

    popTokenStatus();
    return std::make_shared<AnnaAssignmentSyntax>(left, eq, right);

not_assignment:
    revertTokenStatus();
    return gcnAssignment();
}

gcnReturnStatement AnnaParser::parseReturnStatement()
{
    pushTokenStatus();
    gcnToken ret;
    gcnExpression expr;
    gcnEOS eos;

    ret = eatToken(RETURN, "`return'");
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
    revertTokenStatus();
    return gcnReturnStatement();
}


gcnToken AnnaParser::eatToken(bool keepNewlineT)
{
    if (!keepNewlineT) {
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

gcnToken AnnaParser::eatToken(Tokens kind, const char *expected, bool keepNewlineT)
{
    pushTokenStatus();

    gcnToken tok = eatToken(keepNewlineT);

    int row = tok->row();
    int col = tok->col();
    int width = tok->width();
    if (tok->token() != kind) {
        log_print_pos(row, col, _filename);
        if (expected) {
            std::fprintf(__log_out, "Invalid token `");
            std::fputs(tok->text()->c_str(), __log_out);
            std::fprintf(__log_out, "', expected %s\n", expected);
        } else {
            std::fprintf(__log_out, "Invalid token `");
            std::fputs(tok->text()->c_str(), __log_out);
            std::fprintf(__log_out, "'\n");
        }
        log_print_row(row);
        log_print_indicators(col, width);
        revertTokenStatus();
        return gcnToken();
    }

    popTokenStatus();
    return tok;
}

gcnToken AnnaParser::peekToken(int ahead, bool keepNewlineT)
{
    for (unsigned int i = ahead + currentTokenIdx; i < tokens.size(); ++i) {
        if (keepNewlineT)
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



