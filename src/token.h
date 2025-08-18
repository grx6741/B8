#pragma once
#include <stdint.h>

#define MAX_IDENTIFIER_NAME_LEN 255

typedef enum {
    kTokenInt, kTokenIf,
    kTokenIdentifier, kTokenNumber,
    kTokenAssign, kTokenPlus, kTokenCompareEquals,
    kTokenOpenParen, kTokenCloseParen,
    kTokenOpenBrace, kTokenCloseBrace,
    kTokenSemiColon,
    kTokenEOF,
    kTokenCount
} TokenType;

typedef struct {
    TokenType type;
    int row;
    int col;
    union {
	uint8_t value;
	char name[MAX_IDENTIFIER_NAME_LEN];
    };
} token_t;

#define NEW_TOKEN(_type, ...) \
    (token_t) { .type = _type, __VA_ARGS__ }

void LogToken(token_t token);
