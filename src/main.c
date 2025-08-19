#include <stdio.h>

#include "lexer.h"

int main() {

    FILE* f = fopen("input.b8", "r");

    lexer_t L = Lexer(f);
    token_t token;

    while (LexerHasMoreTokens(&L)) {
	token = LexerNextToken(&L);
	LogToken(token);
    }

    fclose(f);

    return 0;
}
