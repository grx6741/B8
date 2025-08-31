#pragma once

#include "lexer.h"
#include "ast.h"

typedef struct {
    lexer_t* L;
    token_t curr;
} parser_t;

// Constructor for parser_t
parser_t Parser(lexer_t* lexer);

// Parses with the grammar :: (program := stmt_list)
ast_node_t* ParseProgram(parser_t* P);
