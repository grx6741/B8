#include "ast.h"
#include "stdlib.h"

#define LOG_ENABLED 1
#define LOG_PREFIX "AST"
#include "util.h"

ast_node_t* ASTnode(ASTnodeType type)
{
    ast_node_t* node = (ast_node_t*)malloc(sizeof(ast_node_t));
    node->type = type;
    return node;
}

void ASTnodeLog(ast_node_t* node, int indent_level)
{
    ast_node_t* curr = node;
    while (curr) {
	switch (curr->type) {
	    case kASTnodeDeclare:
		LOG_INFO("%*sdecl<%s>\n", indent_level * 4, 
			 "", curr->declaration.name);
		break;
	    case kASTnodeAssign:
		LOG_INFO("%*sassign<%s>\n", indent_level * 4, 
			"", curr->assignment.lhs);
		ASTnodeLog(curr->assignment.rhs, indent_level + 1);
		break;
	    case kASTnodeBinaryOp:
		LOG_INFO("%*s%s\n", indent_level * 4, "",
			curr->binary_op.op == kBinaryAdd ? "Add" : "Sub");
		ASTnodeLog(curr->binary_op.lhs, indent_level + 1);
		ASTnodeLog(curr->binary_op.rhs, indent_level + 1);
		break;
	    case kASTnodeIdentifier:
		LOG_INFO("%*sVar<%s>\n", indent_level * 4, 
			"", curr->value.name);
		break;
	    case kASTnodeConstant:
		LOG_INFO("%*sConst<%d>\n", indent_level * 4, 
			"", curr->value.constant);
		break;
	}

	curr = curr->next;
    }
}
