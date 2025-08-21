#include "parser.h"

#define LOG_ENABLED 1
#define LOG_PREFIX "PARSER"
#include "util.h"

#include "assert.h"
#include "string.h"

parser_t Parser(lexer_t *lexer) {
    assert(lexer);
    parser_t P;
    P.L = lexer;
    P.curr = LexerNextToken(P.L);
    return P;
}

// program	    := stmt_list
ast_node_t *ParseProgram(parser_t *P) { return ParseStmtList(P); }

// stmt_list	:= stmt*
// stmt		:= decl | assign | if | block
ast_node_t *ParseStmtList(parser_t *P) {
    ast_node_t *node = NULL;
    ast_node_t *next = NULL;

    while (peek(P).type == kTokenInt ||        // Declaration
	    peek(P).type == kTokenIdentifier || // Assignment
	    peek(P).type == kTokenIf ||         // If
	    peek(P).type == kTokenOpenBrace     // Block
	  ) {
	ast_node_t* stmt = ParseStmt(P);
	if (!node) {
	    node = stmt;
	    next = stmt;
	}
	else {
	    next->next = stmt;
	    next = stmt;
	}
    }

    return node;
}

// stmt		:= decl | assign | if | block
ast_node_t *ParseStmt(parser_t *P) {
    if (peek(P).type == kTokenInt)
	return ParseDecl(P);

    if (peek(P).type == kTokenIdentifier)
	return ParseAssign(P);

    if (peek(P).type == kTokenIf)
	return ParseIf(P);

    if (peek(P).type == kTokenOpenBrace)
	return ParseBlock(P);

    LOG_ERROR(
	    "Invalid Statement. Expected \"int\", \"variable\", \"if\", \"}\"\n");

    return NULL;
}

// decl	    := 'int' identifier ';'
ast_node_t *ParseDecl(parser_t *P) {
    consume(P, kTokenInt);
    token_t tok = consume(P, kTokenIdentifier);
    consume(P, kTokenSemiColon);

    ast_node_t *node = new_ast_node(kASTnodeDeclare);
    strncpy(node->declaration.name, tok.name, MAX_IDENTIFIER_NAME_LEN);

    return node;
}

ast_node_t *ParseAssign(parser_t *P) {}

ast_node_t *ParseBlock(parser_t *P) { return NULL; }

ast_node_t *ParseIf(parser_t *P) {}

// Private

token_t peek(parser_t *P) { return P->curr; }

token_t consume(parser_t *P, TokenType type) {
    assert(P->L);
    if (P->curr.type != type) {
	LOG_ERROR("Expected Token Type %d, but got %d", type, P->curr.type);
    }

    token_t token = P->curr;
    P->curr = LexerNextToken(P->L);
    return token;
}
