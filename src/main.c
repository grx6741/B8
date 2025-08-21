#include <stdio.h>

#include "lexer.h"
#include "parser.h"

#define LOG_ENABLED 1
#include "util.h"

// TODO
int depth = 0;
void dfs(ast_node_t* node)
{
    ast_node_t* curr = node;
    while (curr) {
	LOG_INFO("%d\n", curr->type);
	LOG_INFO("%s\n", curr->declaration.name);

	curr = curr->next;
    }
}

int main() {

    FILE* f = fopen("input.b8", "r");

    lexer_t L = Lexer(f);

    parser_t P = Parser(&L);
    ast_node_t* AST = ParseProgram(&P);
    dfs(AST);

    fclose(f);

    return 0;
}
