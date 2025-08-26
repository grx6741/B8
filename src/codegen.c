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
    if ( node->assignment.rhs->type == kASTnodeBinaryOp ) {
        // c = v op v
        fprintf( out_stream, "NOT IMPLEMENTED: __codegen_assignment_with_binary_op* \n" );
    }
    else {
        // c = v
        if ( node->assignment.rhs->type == kASTnodeIdentifier ) {
            // c = a
            const char* lhs_name = node->assignment.lhs;
            const char* rhs_name = node->assignment.rhs->value.name;

            assert( HTlookup( &scope->addr_table, rhs_name ) );
            assert( HTlookup( &scope->addr_table, lhs_name ) );

            uint8_t lhs_addr = HT_LOOKUP_INT( &scope->addr_table, rhs_name );
            uint8_t rhs_addr = HT_LOOKUP_INT( &scope->addr_table, rhs_name );
            fprintf( out_stream,
                     "; %s(%d) = %s(%d)\n",
                     lhs_name,
                     ctx->stack_pointer,
                     rhs_name,
                     rhs_addr );
            fprintf( out_stream, "mov A M %d\n", rhs_addr );
            fprintf( out_stream, "mov M A %d\n", lhs_addr );
            ctx->stack_pointer--;
        }
        else {
            // c = 20
            const char* id_name = node->assignment.lhs;
            const int constant = node->assignment.rhs->value.constant;

            assert( HTlookup( &scope->addr_table, id_name ) );
            uint8_t id_addr = HT_LOOKUP_INT( &scope->addr_table, id_name );

            fprintf( out_stream,
                     "; %s(%d) = %d\n",
                     id_name,
                     id_addr,
                     node->assignment.rhs->value.constant );
            fprintf( out_stream, "ldi A %d\n", constant );
            fprintf( out_stream, "mov M A %d\n", id_addr );
            ctx->stack_pointer--;
        }
    }
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
