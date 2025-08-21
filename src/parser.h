#pragma once

#include "lexer.h"
#include "ast.h"

typedef struct {
    lexer_t* L;
    token_t curr;
} parser_t;

// Public
parser_t Parser(lexer_t* lexer);
ast_node_t* ParseProgram(parser_t* P);
ast_node_t* ParseStmtList(parser_t* P);
ast_node_t* ParseStmt(parser_t* P);
ast_node_t* ParseBlock(parser_t* P);

ast_node_t* ParseDecl(parser_t* P);
ast_node_t* ParseAssign(parser_t* P);
ast_node_t* ParseExpr(parser_t* P);
ast_node_t* ParseExpr_(parser_t* P, ast_node_t* lhs);
ast_node_t* ParseValue(parser_t* P);
ast_node_t* ParseIf(parser_t* P);

// Private
token_t peek(parser_t* P);
token_t consume(parser_t* P, TokenType type, const char* caller);
