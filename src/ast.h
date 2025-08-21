#pragma once

#include "token.h"

typedef enum {
    kASTnodeDeclare,
    kASTnodeAssign,
    kASTnodeBinaryOp,
    kASTnodeIdentifier,
    kASTnodeConstant,
} ASTnodeType;

typedef struct ast_node_t ast_node_t;
struct ast_node_t {
    ASTnodeType type;
    union {
	struct {
	    char name[MAX_IDENTIFIER_NAME_LEN];
	} declaration;
	struct {
	    char lhs[MAX_IDENTIFIER_NAME_LEN];
	    ast_node_t* rhs; // binary_op node or value node
	} assignment;
	struct {
	    ast_node_t* lhs; // value node
	    ast_node_t* rhs; // value node
	    enum { kBinaryAdd, kBinarySubtract} op;
	} binary_op;
	struct {
	    union {
		char name[MAX_IDENTIFIER_NAME_LEN];
		uint8_t constant;
	    };
	} value;
    };

    ast_node_t* next; // Linked List of Nodes
};

ast_node_t* ASTnode(ASTnodeType type);
void ASTnodeLog(ast_node_t* node, int indent_level);
