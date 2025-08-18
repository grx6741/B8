#include "lexer.h"
#include "ctype.h"

#define LOG_ENABLED 1
#define LOG_PREFIX "LEXER"
#include "util.h"

// Public

lexer_t Lexer(FILE* fd)
{
    return (lexer_t) {
	.fd = fd,
	.row = 0, .col = 0
    };
}

bool LexerHasMoreTokens(const lexer_t* L)
{
    char c = fgetc(L->fd);
    bool has_reached_end = (c == EOF);

    ungetc(c, L->fd);

    return !has_reached_end;
}

token_t LexerNextToken(lexer_t* L)
{
    skipWhitespace(L);

    char c = fgetc(L->fd);
    token_t tok;
    if (c == EOF) {
	tok = NEW_TOKEN(kTokenEOF, .row = L->row, .col = L->col);
    } else {
	tok = NEW_TOKEN(kTokenIdentifier, .row = L->row, .col = L->col);
	tok.name[0] = c;
	tok.name[1] = 0;
    }

    LogToken(tok);

    return tok;
}

// Private

void skipWhitespace(lexer_t* L)
{
    char c = fgetc(L->fd);
    while (c != EOF && isspace(c)) {
	L->col += 1;
	if (c == '\n')
	    L->row += 1;
	c = fgetc(L->fd);
    }
    ungetc(c, L->fd);
}
