#include "codegen.h"
#include "ast.h"
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define LOG_ENABLED 1
#define LOG_PREFIX "CODE_GEN"
#include "util.h"

void __codegen( codegen_context_t* ctx, FILE* out_stream, const ast_node_t* AST );

uint8_t __ctx_get_symbol_addr( codegen_context_t* ctx, const char* symbol_name) {
	int64_t i = ctx->scope_stack_top;

	while (i >= 0) {
		scope_t* scope = &ctx->scope_stack[i];
		ht_element_t* val = HTlookup( &scope->addr_table, symbol_name );
		if (val) return val->int_val;
		i--;
	}

	fprintf(stderr, "Use of Undeclared Symbol \"%s\"\n", symbol_name);
	exit(EXIT_FAILURE);

	return -1;
}

void __codegen_loop( codegen_context_t* ctx, FILE* out_stream, const ast_node_t* node )
{
	ctx->scope_stack_top++;
	fprintf(out_stream, "__loop%d:\n", ctx->loop_block_counter);
	__codegen(ctx, out_stream, node->loop.block);
	fprintf(out_stream, "jmp %%__loop%d\n", ctx->loop_block_counter);
	fprintf(out_stream, "__endloop%d:\n", ctx->loop_block_counter);

	ctx->loop_block_counter++;
}

void __codegen_break( codegen_context_t* ctx, FILE* out_stream, const ast_node_t* node )
{
	fprintf(out_stream, "jmp %%__endloop%d\n", ctx->loop_block_counter);
}

void __codegen_func_call(codegen_context_t* ctx, FILE* out_stream, const ast_node_t* curr)
{
	const char* func_name = curr->func_call.name;
	if ( strcmp(func_name, "out") != 0) {
		LOG_ERROR("Unsupported Function \"%s\"\n", func_name);
	}

	const char* arg_name = curr->func_call.arg->value.name;
	uint8_t arg_addr = __ctx_get_symbol_addr(ctx, arg_name);

	fprintf(out_stream, "mov A M %d; 'A = M[%s]\n", arg_addr, arg_name);
	fprintf(out_stream, "out 0; out(%s)\n", arg_name);
}

void __codegen_declaration( codegen_context_t* ctx, FILE* out_stream, const ast_node_t* node )
{
    scope_t* scope = &ctx->scope_stack[ctx->scope_stack_top];

    if ( HTlookup( &scope->addr_table, node->declaration.name ) != NULL ) {
        LOG_ERROR( "Variable \"%s\" Declared Twice\n", node->declaration.name );
        assert( 0 );
    }

    HTinsertInt( &scope->addr_table, node->declaration.name, ctx->stack_pointer-- );
}

// Evaluates the Expression and places the result in A
// But if the Op is a comparison, it won't update A
// it'll just update the flags
// TODO : So var = a > b is not Supported Now
void __codegen_expression( codegen_context_t* ctx, FILE* out_stream, const ast_node_t* node )
{
    if ( node->type == kASTnodeBinaryOp ) {
        // c = v1 op v2 || c1 op v2 || v1 op c2 || v1 + v2
        // where v{1,2} is a variable and c{1,2} is a constant
        // get c1,v1 and put it into A

        if ( node->binary_op.lhs->type == kASTnodeIdentifier ) {
            const char* v1_name = node->binary_op.lhs->value.name;

            // assert( HTlookup( &scope->addr_table, v1_name ) );
            // uint8_t v1_addr = HT_LOOKUP_INT( &scope->addr_table, v1_name );
			uint8_t v1_addr = __ctx_get_symbol_addr(ctx, v1_name);

            // Move value of first RHS var into A
            fprintf( out_stream, "mov A M %d; 'A = M[%s]\n", v1_addr, v1_name );
        }
        else {
            const int c1 = node->binary_op.lhs->value.constant;

            // Move the constant value into A
            fprintf( out_stream, "ldi A %d\n", c1 );
        }

        // get c2,v2 and put it into B
        if ( node->binary_op.rhs->type == kASTnodeIdentifier ) {
            const char* v2_name = node->binary_op.rhs->value.name;

            // assert( HTlookup( &scope->addr_table, v2_name ) );
            // uint8_t v2_addr = HT_LOOKUP_INT( &scope->addr_table, v2_name );
            uint8_t v2_addr = __ctx_get_symbol_addr( ctx, v2_name );

            // Move value of first RHS var into A
            fprintf( out_stream, "mov B M %d; 'B = M[%s]\n", v2_addr, v2_name );
        }
        else {
            const int c2 = node->binary_op.rhs->value.constant;

            // Move the constant value into A
            fprintf( out_stream, "ldi B %d\n", c2 );
        }

        // call the operation
        switch ( node->binary_op.op ) {
            case kBinaryAdd:
                fprintf( out_stream, "add\n" );
                break;
            case kBinarySub:
                fprintf( out_stream, "sub\n" );
                break;
			case kBinaryLT:
			case kBinaryLE:
			case kBinaryGT:
			case kBinaryGE:
			case kBinaryNE:
			case kBinaryEQ:
                fprintf( out_stream, "cmp\n" );
				break;
            default:
                LOG_WARN( "Unsupported Binary Op %s\n",
                          ASTnodeBinaryTypeToString( node->binary_op.op ) );
        }
    }
    else {
        // c = v
        if ( node->type == kASTnodeIdentifier ) {
            // c = variable
            const char* rhs_name = node->value.name;

            // assert( HTlookup( &scope->addr_table, rhs_name ) );
            // uint8_t rhs_addr = HT_LOOKUP_INT( &scope->addr_table, rhs_name );
            uint8_t rhs_addr = __ctx_get_symbol_addr(ctx, rhs_name);

            fprintf( out_stream, "mov A M %d; 'A = %s\n", rhs_addr, rhs_name );
        }
        else {
            // c = constant
            const int constant = node->value.constant;
            fprintf( out_stream, "ldi A %d\n", constant );
        }
    }
}
 
void __codegen_assignment( codegen_context_t* ctx, FILE* out_stream, const ast_node_t* node )
{
    const char* lhs_name = node->assignment.lhs;

    // assert( HTlookup( &scope->addr_table, lhs_name ) );
    // uint8_t lhs_addr = HT_LOOKUP_INT( &scope->addr_table, lhs_name );
    uint8_t lhs_addr = __ctx_get_symbol_addr( ctx, lhs_name );

	__codegen_expression(ctx, out_stream, node->assignment.rhs);

    // Update the LHS's value in RAM
    fprintf( out_stream, "mov M A %d; M[%s] = 'A\n", lhs_addr, lhs_name );
}

void __codegen_if( codegen_context_t* ctx, FILE* out_stream, const ast_node_t* node )
{
	// asm for condition
	ast_node_t* condition = node->if_node.condition;

	uint32_t if_block_num = ctx->if_block_counter++;

	__codegen_expression(ctx, out_stream, condition);

	// the condition expression could've been if (1) or if (1 > 3)
	if ( condition->type == kASTnodeConstant || condition->type == kASTnodeIdentifier )
	{
		fprintf( out_stream, "ldi B 0\n" );
		fprintf( out_stream, "cmp\n" );
		fprintf( out_stream, "je %%__else%d\n", if_block_num);
	}


	// if not condition, jump to else block
	switch (condition->binary_op.op) {
		case kBinaryAdd: 
		case kBinarySub: 
			break;
		case kBinaryLT: 
            fprintf( out_stream, "je  %%__else%d\n", if_block_num);
            fprintf( out_stream, "jnc %%__else%d\n", if_block_num);
			break;
		case kBinaryLE: 
            fprintf( out_stream, "je  %%__if%d\n", if_block_num);
            fprintf( out_stream, "jnc %%__else%d\n", if_block_num);
			break;
		case kBinaryGT: 
            fprintf( out_stream, "je %%__else%d\n", if_block_num);
            fprintf( out_stream, "jc %%__else%d\n", if_block_num);
			break;
		case kBinaryGE: 
            fprintf( out_stream, "je %%__if%d\n", if_block_num);
            fprintf( out_stream, "jc %%__else%d\n", if_block_num);
			break;
		case kBinaryNE: 
            fprintf( out_stream, "je %%__else%d\n", if_block_num);
			break;
		case kBinaryEQ: 
            fprintf( out_stream, "jne %%__else%d\n", if_block_num);
			break;
		default:
			LOG_WARN("Unsupported Binary Op %s\n", ASTnodeBinaryTypeToString(condition->binary_op.op));
	}
	
	// asm for if block
	ctx->scope_stack_top++;
	fprintf(out_stream, "__if%d:\n", if_block_num);
	__codegen(ctx, out_stream, node->if_node.if_block);
	fprintf(out_stream, "jmp %%__endif%d\n", if_block_num);

	// asm for else block
	ctx->scope_stack_top++;
	fprintf(out_stream, "__else%d:\n", if_block_num);
	__codegen(ctx, out_stream, node->if_node.else_block);
	fprintf(out_stream, "__endif%d:\n", if_block_num);
}

void __codegen( codegen_context_t* ctx, FILE* out_stream, const ast_node_t* AST )
{
    scope_t* scope = &ctx->scope_stack[ctx->scope_stack_top];
    HTdelete( &scope->addr_table );
    // save the stack pointer for this scope
    scope->frame_pointer = ctx->stack_pointer;

	const ast_node_t* curr = AST;
	if (curr) {
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
				case kASTnodeBreak:
					__codegen_break(ctx, out_stream, curr);
					break;
				case kASTnodeFuncCall:
					__codegen_func_call(ctx, out_stream, curr);
					break;
				case kASTnodeLoop:
					__codegen_loop(ctx, out_stream, curr);
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
	}

    HTdelete( &scope->addr_table );
    // Restore the stack pointer
    ctx->stack_pointer = scope->frame_pointer;
    // TODO: Add assembly code to restore the internal stack pointer
    ctx->scope_stack_top--;
}

void Codegen( FILE* out_stream, const ast_node_t* AST )
{
    codegen_context_t ctx = { .stack_pointer = 0xFF };
	fprintf(out_stream, ".text\n");
	__codegen(&ctx, out_stream, AST);
	fprintf(out_stream, "hlt\n");
}
