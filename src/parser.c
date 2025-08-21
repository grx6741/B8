#include "parser.h"

#define LOG_ENABLED 1
#define LOG_PREFIX "PARSER"
#include "util.h"

#include "assert.h"
#include "string.h"

#define CONSUME(P, type) consume(P, type, __func__)

parser_t Parser(lexer_t* lexer) {
    assert(lexer);
    parser_t P;
    P.L = lexer;
    P.curr = LexerNextToken(P.L);
    return P;
}

// program	    := stmt_list
ast_node_t* ParseProgram(parser_t* P) { 
    LOG_INFO("Parsing Program\n");
    return ParseStmtList(P);
}

// stmt_list	:= stmt*
// stmt		:= decl | assign | if | block
ast_node_t* ParseStmtList(parser_t* P) {
    LOG_INFO("Parsing StmtList\n");
    ast_node_t* node = NULL;
    ast_node_t* next = NULL;

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
ast_node_t* ParseStmt(parser_t* P) {
    LOG_INFO("Parsing Stmt\n");
    if (peek(P).type == kTokenInt)
	return ParseDecl(P);

    if (peek(P).type == kTokenIdentifier)
	return ParseAssign(P);

    if (peek(P).type == kTokenIf)
	return ParseIf(P);

    if (peek(P).type == kTokenOpenBrace)
	return ParseBlock(P);

    LOG_ERROR("Invalid Statement."
	      "Expected \"int\", \"variable\", \"if\", \"{\"\n");

    return NULL;
}

// decl	    := 'int' identifier ';'
ast_node_t* ParseDecl(parser_t* P) {
    LOG_INFO("Parsing Decl\n");
    CONSUME(P, kTokenInt);
    token_t tok = CONSUME(P, kTokenIdentifier);
    CONSUME(P, kTokenSemiColon);

    ast_node_t* node = ASTnode(kASTnodeDeclare);
    strncpy(node->declaration.name, 
	    tok.name, MAX_IDENTIFIER_NAME_LEN);

    return node;
}

// assign := identifier '=' expr ';'
ast_node_t* ParseAssign(parser_t* P) { 
    LOG_INFO("Parsing Assigment\n");
    ast_node_t* node = ASTnode(kASTnodeAssign);

    token_t iden = CONSUME(P, kTokenIdentifier);
    strncpy(node->assignment.lhs, 
	    iden.name, MAX_IDENTIFIER_NAME_LEN);

    CONSUME(P, kTokenAssign);

    node->assignment.rhs = ParseExpr(P);

    CONSUME(P, kTokenSemiColon);

    return node;
}

// expr := value expr_
ast_node_t* ParseExpr(parser_t* P) {
    LOG_INFO("Parsing Expr\n");
    ast_node_t* lhs = ParseValue(P);
    return ParseExpr_(P, lhs);
}

// expr_ := (op value) | <empty>
ast_node_t* ParseExpr_(parser_t* P, ast_node_t* lhs) {
    LOG_INFO("Parsing Expr_\n");
    if (peek(P).type != kTokenPlus && peek(P).type != kTokenMinus)
	return lhs;

    ast_node_t* node = ASTnode(kASTnodeBinaryOp);
    node->binary_op.lhs = lhs;
    if (peek(P).type == kTokenPlus) {
	node->binary_op.op = kBinaryAdd;
	CONSUME(P, kTokenPlus);
    } else if (peek(P).type == kTokenMinus) {
	node->binary_op.op = kBinarySubtract;
	CONSUME(P, kTokenMinus);
    } else {
	LOG_ERROR("Invalid Binary Operator at %d,%d. Only +,- supported\n",
		P->L->row, P->L->col);
    }

    node->binary_op.rhs = ParseValue(P);
    return node;
}

// value := identifier | constant
ast_node_t* ParseValue(parser_t* P) {
    LOG_INFO("Parsing Value\n");
    if (peek(P).type == kTokenIdentifier) {
	token_t iden = CONSUME(P, kTokenIdentifier);
	ast_node_t* node = ASTnode(kASTnodeIdentifier);

	strncpy(node->value.name, 
		iden.name, MAX_IDENTIFIER_NAME_LEN);
	
	return node;
    }

    if (peek(P).type == kTokenNumber) {
	token_t number = CONSUME(P, kTokenNumber);
	ast_node_t* node = ASTnode(kASTnodeConstant);

	node->value.constant = number.value;
	return node;
    }

    LOG_ERROR("Expected Constant or Identifier at %d,%d\n", 
	    P->L->row, P->L->col);
}

ast_node_t* ParseBlock(parser_t* P) { return NULL; }

ast_node_t* ParseIf(parser_t* P) { return NULL; }

// Private

token_t peek(parser_t* P) { return P->curr; }

token_t consume(parser_t* P, TokenType type, const char* caller) {
    assert(P->L);
    if (peek(P).type != type) {
	LOG_ERROR("[%s] Expected Token Type %s, but got %s at %d,%d\n", 
		caller, TokenTypeToStr(type), 
			TokenTypeToStr(P->curr.type), 
			P->L->row, P->L->col);
    }

    token_t token = peek(P);
    P->curr = LexerNextToken(P->L);
    return token;
}
