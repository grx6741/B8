#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "token.h"

typedef struct {
    FILE* fd;
    int row;
    int col;
} lexer_t;

// Public

lexer_t Lexer(FILE* fd);
bool	LexerHasMoreTokens(const lexer_t* L);
token_t LexerNextToken(lexer_t* L);

// Private
void skipWhitespace(lexer_t* L);
char getChar(lexer_t* L);
void ungetChar(lexer_t* L, char c);
