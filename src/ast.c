#include "ast.h"
#include "stdlib.h"

ast_node_t* new_ast_node(ASTnodeType type)
{
    ast_node_t* node = (ast_node_t*)malloc(sizeof(ast_node_t));
    node->type = type;
    return node;
}
