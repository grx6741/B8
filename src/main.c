#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "codegen.h"

#include <assert.h>

char* shift_args(int* argc, char*** argv)
{
    if (*argc == 0) return NULL;

    char* arg = **argv;     // Take the first argument
    (*argv)++;              // Advance the pointer
    (*argc)--;              // Decrease count
    return arg;
}

void usage(const char* prog_name)
{
	printf("Usage: %s <input_file> <output_file>\n", prog_name);
}

int main(int argc, char* argv[])
{
	// Shift the program name
	const char* prog_name = shift_args(&argc, &argv);

	const char* input_file = shift_args(&argc, &argv);
	if (!input_file) {
		printf("Did not provide input file!\n");
		usage(prog_name);
		exit(EXIT_FAILURE);
	}

	const char* output_file = shift_args(&argc, &argv);
	if (!output_file) {
		printf("Did not provide output file!\n");
		usage(prog_name);
		exit(EXIT_FAILURE);
	}

	printf("%s %s %s\n", prog_name, input_file, output_file);

    FILE* f = fopen( input_file, "r" );

    lexer_t L = Lexer( f );

    parser_t P = Parser( &L );
    ast_node_t* AST = ParseProgram( &P );

    FILE* out_file = fopen( output_file , "w" );
    ASTnodeLog( AST, 0 );

    Codegen( out_file , AST );

    fclose( out_file );
    fclose( f );

    return 0;
}
