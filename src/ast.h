#pragma once

#include "token.h"
#include <stdbool.h>

typedef enum
{
    kASTnodeDeclare,
    kASTnodeAssign,
    kASTnodeBinaryOp,
    kASTnodeIdentifier,
    kASTnodeConstant,
    kASTnodeIf,
} ASTnodeType;
const char* ASTnodeTypeToString( ASTnodeType type );

typedef enum
{
    kBinaryAdd, // +
    kBinarySub, // -
    kBinaryLT,  // <
    kBinaryLE,  // <=
    kBinaryGT,  // >
    kBinaryGE,  // >=
    kBinaryNE,  // !=
    kBinaryEQ,  // =
    kBinaryTypeCount
} ASTnodeBinaryType;
const char* ASTnodeBinaryTypeToString( ASTnodeBinaryType type );

typedef struct ast_node_t ast_node_t;

typedef struct
{
    char name[MAX_IDENTIFIER_NAME_LEN];
} ASTdeclarationNode;

typedef struct
{
    char lhs[MAX_IDENTIFIER_NAME_LEN];
    ast_node_t* rhs; // binary_op node or value node
} ASTassignmentNode;

typedef struct
{
    ast_node_t* lhs; // value node
    ast_node_t* rhs; // value node
    ASTnodeBinaryType op;
} ASTbinaryOpNode;

typedef struct
{
    union
    {
        char name[MAX_IDENTIFIER_NAME_LEN];
        uint8_t constant;
    };
} ASTvalueNode;

typedef struct
{
    ast_node_t* condition;  // expr
    ast_node_t* if_block;   // stmt_list
    ast_node_t* else_block; // stmt_list
} ASTifNode;

struct ast_node_t
{
    ASTnodeType type;
    union
    {
        ASTdeclarationNode declaration;
        ASTassignmentNode assignment;
        ASTbinaryOpNode binary_op;
        ASTvalueNode value;
        ASTifNode if_block;
    };
    // Linked List of Nodes
    ast_node_t* next;
};

ast_node_t* ASTnode( ASTnodeType type );
void ASTnodeLog( ast_node_t* node, int indent_level );
