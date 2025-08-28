#include "codegen.h"
#include <stdarg.h>
#include <assert.h>

#define LOG_ENABLED 1
#define LOG_PREFIX "CODE_GEN"
#include "util.h"

void __codegen_declaration( codegen_context_t* ctx, FILE* out_stream, const ast_node_t* node )
{
    scope_t* scope = &ctx->scope_stack[ctx->scope_stack_top];

    if ( HTlookup( &scope->addr_table, node->declaration.name ) != NULL ) {
        LOG_ERROR( "Variable %s Declared Twice\n", node->declaration.name );
        assert( 0 );
    }

    HTinsertInt( &scope->addr_table, node->declaration.name, ctx->stack_pointer-- );
}

void __codegen_assignment( codegen_context_t* ctx, FILE* out_stream, const ast_node_t* node )
{
    scope_t* scope = &ctx->scope_stack[ctx->scope_stack_top];
    const char* lhs_name = node->assignment.lhs;

    assert( HTlookup( &scope->addr_table, lhs_name ) );
    uint8_t lhs_addr = HT_LOOKUP_INT( &scope->addr_table, lhs_name );

    if ( node->assignment.rhs->type == kASTnodeBinaryOp ) {
        // c = v1 op v2 || c1 op v2 || v1 op c2 || v1 + v2
        // where v{1,2} is a variable and c{1,2} is a constant
        // get c1,v1 and put it into A

        ast_node_t* expr = node->assignment.rhs;
        if ( expr->binary_op.lhs->type == kASTnodeIdentifier ) {
            const char* v1_name = expr->binary_op.lhs->value.name;

            assert( HTlookup( &scope->addr_table, v1_name ) );
            uint8_t v1_addr = HT_LOOKUP_INT( &scope->addr_table, v1_name );

            // Move value of first RHS var into A
            fprintf( out_stream, "mov A M %d; 'A = M[%s]\n", v1_addr, v1_name );
        }
        else {
            const int c1 = expr->binary_op.lhs->value.constant;

            // Move the constant value into A
            fprintf( out_stream, "ldi A %d\n", c1 );
        }

        // get c2,v2 and put it into B
        if ( expr->binary_op.rhs->type == kASTnodeIdentifier ) {
            const char* v2_name = expr->binary_op.rhs->value.name;

            assert( HTlookup( &scope->addr_table, v2_name ) );
            uint8_t v2_addr = HT_LOOKUP_INT( &scope->addr_table, v2_name );

            // Move value of first RHS var into A
            fprintf( out_stream, "mov B M %d; 'B = M[%s]\n", v2_addr, v2_name );
        }
        else {
            const int c2 = expr->binary_op.lhs->value.constant;

            // Move the constant value into A
            fprintf( out_stream, "ldi B %d\n", c2 );
        }

        // call the operation
        switch ( expr->binary_op.op ) {
            case kBinaryAdd:
                fprintf( out_stream, "add\n" );
                break;
            case kBinarySub:
                fprintf( out_stream, "sub\n" );
                break;
            default:
                LOG_WARN( "Unsupported Binary Op %s\n",
                          ASTnodeBinaryTypeToString( expr->binary_op.op ) );
        }
    }
    else {
        // c = v
        if ( node->assignment.rhs->type == kASTnodeIdentifier ) {
            // c = variable
            const char* rhs_name = node->assignment.rhs->value.name;

            assert( HTlookup( &scope->addr_table, rhs_name ) );
            uint8_t rhs_addr = HT_LOOKUP_INT( &scope->addr_table, rhs_name );

            fprintf( out_stream, "mov A M %d; 'A = %s\n", rhs_addr, rhs_name );
        }
        else {
            // c = constant
            const int constant = node->assignment.rhs->value.constant;
            fprintf( out_stream, "ldi A %d\n", constant );
        }
    }

    // Update the LHS's value in RAM
    fprintf( out_stream, "mov M A %d; M[%s] = 'A\n", lhs_addr, lhs_name );
}

void __codegen_if( codegen_context_t* ctx, FILE* out_stream, const ast_node_t* node )
{
    fprintf( out_stream, "NOT IMPLEMENTED: __codegen_if* \n" );
}

void Codegen( codegen_context_t* ctx, FILE* out_stream, const ast_node_t* AST )
{
    scope_t* scope = &ctx->scope_stack[ctx->scope_stack_top];
    HTdelete( &scope->addr_table );
    // save the stack pointer for this scope
    scope->frame_pointer = ctx->stack_pointer;

    const ast_node_t* curr = AST;
    while ( curr ) {
        switch ( curr->type ) {
            case kASTnodeDeclare:
                __codegen_declaration( ctx, out_stream, curr );
                break;
            case kASTnodeAssign:
                __codegen_assignment( ctx, out_stream, curr );
                break;
            case kASTnodeIf:
                __codegen_if( ctx, out_stream, curr );
                break;
            // the rest shouldn't be out in the
            // open, so i dont explicitly handle
            // them for now
            case kASTnodeBinaryOp:
                break;
            case kASTnodeIdentifier:
                break;
            case kASTnodeConstant:
                break;
        }
        curr = curr->next;
    }

    HTlog( &scope->addr_table );

    HTdelete( &scope->addr_table );
    // Restore the stack pointer
    ctx->stack_pointer = scope->frame_pointer;
    // TODO: Add assembly code to restore the internal stack pointer
    ctx->scope_stack_top--;
}
