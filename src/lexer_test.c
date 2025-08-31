#include "string.h"

#define BUFFER(str) fmemopen((void*)(str), strlen((str)), "r")

TEST(LexerVariableDeclaration) {
    const char* input = "	int \t\t a   \n ;  ";
    lexer_t L = Lexer(BUFFER(input), "temp");

    FAIL_IF_NOT(LexerHasMoreTokens(&L), "Expected 3 Tokens\n");
    token_t int_ = LexerNextToken(&L);

    FAIL_IF_NOT(int_.type == kTokenInt, 
	    "Expected 'int' but got %s\n", TokenTypeToStr(int_.type));

    FAIL_IF_NOT(LexerHasMoreTokens(&L), 
	    "Expected 2 more Tokens after 'int'\n");

    token_t identifier = LexerNextToken(&L);

    FAIL_IF_NOT(identifier.type == kTokenIdentifier, 
	    "Expected 'identifier' but got %s\n", 
	    TokenTypeToStr(identifier.type));

    FAIL_IF_NOT(strncmp(identifier.name, "a", MAX_IDENTIFIER_NAME_LEN) == 0, 
	    "Expected identifier's name to be 'a' but got %s\n", 
	    identifier.name);

    FAIL_IF_NOT(LexerHasMoreTokens(&L), "Expected 1 more Tokens after indentifier 'a'\n");
    token_t semicolon = LexerNextToken(&L);
    FAIL_IF_NOT(semicolon.type == kTokenSemiColon, 
	    "Expected 'Semicolon' but got %s\n", TokenTypeToStr(semicolon.type));

    return TEST_PASSED();
}

TEST(LexerVariableAssignment) {
    const char* input = "  \na \t=\t20  ;";
    lexer_t L = Lexer(BUFFER(input), "temp");

    FAIL_IF_NOT(LexerHasMoreTokens(&L), "Expected 4 Tokens\n");
    token_t id = LexerNextToken(&L);

    FAIL_IF_NOT(id.type == kTokenIdentifier, 
	    "Expected 'identifier' but got %s\n", TokenTypeToStr(id.type));
    FAIL_IF_NOT(strncmp(id.name, "a", MAX_IDENTIFIER_NAME_LEN) == 0, 
	    "Expected identifier's name to be 'a' but got %s\n", 
	    id.name);

    FAIL_IF_NOT(LexerHasMoreTokens(&L), 
	    "Expected 3 more Tokens after 'identifier'\n");

    token_t equals = LexerNextToken(&L);

    FAIL_IF_NOT(equals.type == kTokenAssign, 
	    "Expected 'assign' but got %s\n", 
	    TokenTypeToStr(equals.type));

    FAIL_IF_NOT(LexerHasMoreTokens(&L), "Expected 2 more Tokens after equals\n");

    token_t number = LexerNextToken(&L);
    FAIL_IF_NOT(number.type == kTokenNumber, 
	    "Expected 'Number' but got %s\n", TokenTypeToStr(number.type));
    FAIL_IF_NOT(number.value == 20,
	    "Expected identifier's value to be '20' but got %d\n", 
	    number.value);

    FAIL_IF_NOT(LexerHasMoreTokens(&L), "Expected 1 more Tokens after number '20'\n");
    token_t semicolon = LexerNextToken(&L);
    FAIL_IF_NOT(semicolon.type == kTokenSemiColon, 
	    "Expected 'Semicolon' but got %s\n", TokenTypeToStr(semicolon.type));

    return TEST_PASSED();
}
