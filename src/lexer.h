#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "token.h"

typedef struct {
    FILE* fd;
	const char* file_name;
    int row;
    int col;
} lexer_t;

lexer_t Lexer(FILE* fd, const char* file_name);
bool	LexerHasMoreTokens(const lexer_t* L);
token_t LexerNextToken(lexer_t* L);
