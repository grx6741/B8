#include "ast.h"
#include "stdlib.h"

#define LOG_ENABLED 0
#define LOG_PREFIX "AST"
#include "util.h"

ast_node_t* ASTnode( ASTnodeType type )
{
    ast_node_t* node = ( ast_node_t* ) malloc( sizeof( ast_node_t ) );
    node->type = type;
    return node;
}

void ASTnodeLog( ast_node_t* node, int indent_level )
{
    ast_node_t* curr = node;
    while ( curr ) {
        switch ( curr->type ) {
            case kASTnodeDeclare:
                LOG_INFO( "%*sdecl<%s>\n", indent_level * 4, "", curr->declaration.name );
                break;
            case kASTnodeAssign:
                LOG_INFO( "%*sassign<%s>\n", indent_level * 4, "", curr->assignment.lhs );
                ASTnodeLog( curr->assignment.rhs, indent_level + 1 );
                break;
            case kASTnodeBinaryOp:
                LOG_INFO( "%*s%s\n",
                          indent_level * 4,
                          "",
                          ASTnodeBinaryTypeToString( curr->binary_op.op ) );
                ASTnodeLog( curr->binary_op.lhs, indent_level + 1 );
                ASTnodeLog( curr->binary_op.rhs, indent_level + 1 );
                break;
            case kASTnodeIdentifier:
                LOG_INFO( "%*sVar<%s>\n", indent_level * 4, "", curr->value.name );
                break;
            case kASTnodeConstant:
                LOG_INFO( "%*sConst<%d>\n", indent_level * 4, "", curr->value.constant );
                break;
            case kASTnodeIf:
                LOG_INFO( "%*sIf:\n", indent_level * 4, "" );
                ASTnodeLog( curr->if_node.condition, indent_level + 1 );
                LOG_INFO( "%*sThen:\n", indent_level * 4, "" );
                ASTnodeLog( curr->if_node.if_block, indent_level + 1 );
                LOG_INFO( "%*sElse:\n", indent_level * 4, "" );
                ASTnodeLog( curr->if_node.else_block, indent_level + 1 );
                LOG_INFO( "%*sEndif:\n", indent_level * 4, "" );
                break;
        }

        curr = curr->next;
    }
}

const char* ASTnodeBinaryTypeToString( ASTnodeBinaryType type )
{
    switch ( type ) {
        case kBinaryAdd:
            return "kBinaryAdd";
        case kBinarySub:
            return "kBinarySub";
        case kBinaryEQ:
            return "kBinaryEq";
        case kBinaryLT:
            return "kBinaryLt";
        case kBinaryLE:
            return "kBinaryLte";
        case kBinaryGT:
            return "kBinaryGt";
        case kBinaryGE:
            return "kBinaryGte";
        case kBinaryNE:
            return "kBinaryNe";
        case kBinaryTypeCount:
            return "kBinaryInvalid";
    }

    return NULL;
}

const char* ASTnodeTypeToString( ASTnodeType type )
{
    switch ( type ) {
        case kASTnodeDeclare:
            return "kASTnodeDeclare";
        case kASTnodeAssign:
            return "kASTnodeAssign";
        case kASTnodeBinaryOp:
            return "kASTnodeBinaryOp";
        case kASTnodeIdentifier:
            return "kASTnodeIdentifier";
        case kASTnodeConstant:
            return "kASTnodeConstant";
        case kASTnodeIf:
            return "kASTnodeIf";
    }

    return NULL;
}
