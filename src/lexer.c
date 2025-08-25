#include "lexer.h"

#include "ctype.h"
#include "string.h"
#include "stdlib.h"

// Public

lexer_t Lexer( FILE* fd )
{
    return ( lexer_t ) { .fd = fd, .row = 1, .col = 1 };
}

bool LexerHasMoreTokens( const lexer_t* L )
{
    char c = fgetc( L->fd );
    bool has_reached_end = ( c == EOF );

    ungetc( c, L->fd );

    return !has_reached_end;
}

token_t LexerNextToken( lexer_t* L )
{
    skipWhitespace( L );

    char c = getChar( L );

    if ( c == '/' ) {
        // Can be comment
        char next = getChar( L );
        if ( next == '/' ) {
            // Skip entire line, cuz this is a comment
            while ( getChar( L ) != '\n' )
                ;
            L->row += 1;
            L->col = 1;

            return LexerNextToken( L );
        }
        ungetChar( L, next );
        // Can be a Division Operator. Not supported for now
    }
    else if ( isalpha( c ) ) {
        char id_name[MAX_IDENTIFIER_NAME_LEN];
        size_t id_name_len = 0;
        int start_col = L->col - 1;
        while ( isalnum( c ) ) {
            // TODO : Will Cause Buffer overflow if the identifier
            // name is more than 'MAX_IDENTIFIER_NAME_LEN'
            id_name[id_name_len++] = c;
            c = getChar( L );
        }
        id_name[id_name_len] = '\0';
        ungetChar( L, c );

        if ( strcmp( id_name, "if" ) == 0 )
            return NEW_TOKEN( kTokenIf, .row = L->row, .col = start_col );
        if ( strcmp( id_name, "else" ) == 0 )
            return NEW_TOKEN( kTokenElse, .row = L->row, .col = start_col );
        if ( strcmp( id_name, "int" ) == 0 )
            return NEW_TOKEN( kTokenInt, .row = L->row, .col = start_col );

        token_t token = NEW_TOKEN( kTokenIdentifier, .row = L->row, .col = start_col );
        strncpy( token.name, id_name, MAX_IDENTIFIER_NAME_LEN );

        return token;
    }
    else if ( isdigit( c ) ) {
        char number[MAX_IDENTIFIER_NAME_LEN];
        size_t number_len = 0;
        int start_col = L->col - 1;
        while ( isdigit( c ) ) {
            // TODO : Will Cause Buffer overflow if the identifier
            // name is more than 'MAX_IDENTIFIER_NAME_LEN'
            number[number_len++] = c;
            c = getChar( L );
        }
        number[number_len] = '\0';
        ungetChar( L, c );

        // TODO : Better Error Handling
        return NEW_TOKEN(
            kTokenNumber, .value = strtol( number, NULL, 10 ), .row = L->row, .col = start_col );
    }
    else if ( c == '=' ) {
        char next = getChar( L );
        if ( next == '=' )
            return NEW_TOKEN( kTokenCompareEquals, .row = L->row, .col = L->col - 2 );
        ungetChar( L, next );
        return NEW_TOKEN( kTokenAssign, .row = L->row, .col = L->col - 1 );
    }
    else if ( c == '>' ) {
        char next = getChar( L );
        if ( next == '=' )
            return NEW_TOKEN( kTokenCompareGreaterThanEquals, .row = L->row, .col = L->col - 2 );
        ungetChar( L, next );
        return NEW_TOKEN( kTokenCompareGreaterThan, .row = L->row, .col = L->col - 1 );
    }
    else if ( c == '<' ) {
        char next = getChar( L );
        if ( next == '=' )
            return NEW_TOKEN( kTokenCompareLessThanEquals, .row = L->row, .col = L->col - 2 );
        ungetChar( L, next );
        return NEW_TOKEN( kTokenCompareLessThan, .row = L->row, .col = L->col - 1 );
    }
    else if ( c == '!' ) {
        char next = getChar( L );
        if ( next == '=' )
            return NEW_TOKEN( kTokenCompareNotEquals, .row = L->row, .col = L->col - 2 );
        ungetChar( L, next );
        // TODO : When '!' unary support is to be added, add it here
        return NEW_TOKEN( kTokenInvalid, .row = L->row, .col = L->col - 1 );
    }
    else if ( c == '(' )
        return NEW_TOKEN( kTokenOpenParen, .row = L->row, .col = L->col - 1 );
    else if ( c == ')' )
        return NEW_TOKEN( kTokenCloseParen, .row = L->row, .col = L->col - 1 );
    else if ( c == '{' )
        return NEW_TOKEN( kTokenOpenBrace, .row = L->row, .col = L->col - 1 );
    else if ( c == '}' )
        return NEW_TOKEN( kTokenCloseBrace, .row = L->row, .col = L->col - 1 );
    else if ( c == ';' )
        return NEW_TOKEN( kTokenSemiColon, .row = L->row, .col = L->col - 1 );
    else if ( c == '+' )
        return NEW_TOKEN( kTokenPlus, .row = L->row, .col = L->col - 1 );
    else if ( c == '-' )
        return NEW_TOKEN( kTokenMinus, .row = L->row, .col = L->col - 1 );
    else if ( c == EOF )
        return NEW_TOKEN( kTokenEOF, .row = L->row, .col = L->col - 1 );

    return NEW_TOKEN( kTokenInvalid, .row = L->row, .col = L->col, .name = { c } );
}

// Private

void skipWhitespace( lexer_t* L )
{
    char c = getChar( L );
    while ( c != EOF && isspace( c ) ) {
        if ( c == '\n' ) {
            L->row += 1;
            L->col = 1;
        }
        c = getChar( L );
    }
    ungetChar( L, c );
}

char getChar( lexer_t* L )
{
    char c = fgetc( L->fd );
    L->col += 1;
    return c;
}

void ungetChar( lexer_t* L, char c )
{
    ungetc( c, L->fd );
    L->col -= 1;
}
