#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "codegen.h"

#include <assert.h>

int main()
{
    FILE* f = fopen( "input.b8", "r" );

    lexer_t L = Lexer( f );

    parser_t P = Parser( &L );
    ast_node_t* AST = ParseProgram( &P );

    ASTnodeLog( AST, 0 );

    codegen_context_t ctx = { .stack_pointer = 0xFF };
    Codegen( &ctx, stdout, AST );

    fclose( f );

    return 0;
}
