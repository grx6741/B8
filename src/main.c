#include <stdio.h>

#include "lexer.h"
#include "parser.h"

int main() {

    FILE* f = fopen("input.b8", "r");

    lexer_t L = Lexer(f);

    parser_t P = Parser(&L);
    ast_node_t* AST = ParseProgram(&P);
    ASTnodeLog(AST, 0);

    fclose(f);

    return 0;
}
