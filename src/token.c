#include "token.h"

#define LOG_ENABLED 1
#define LOG_PREFIX "TOKEN"
#include "util.h"

void LogToken( token_t token )
{
    LOG_INFO( "%s[%d, %d]\n", TokenTypeToStr( token.type ), token.row, token.col );
}

const char* TokenTypeToStr( TokenType type )
{
    switch ( type ) {
        case kTokenElse:
            return "else";
        case kTokenCompareNotEquals:
            return "!=";
        case kTokenCompareLessThan:
            return "<";
        case kTokenCompareLessThanEquals:
            return "<=";
        case kTokenCompareGreaterThan:
            return ">";
        case kTokenCompareGreaterThanEquals:
            return ">=";
        case kTokenInt:
            return "int";
        case kTokenIf:
            return "if";
        case kTokenIdentifier:
            return "identifier";
        case kTokenNumber:
            return "number";
        case kTokenAssign:
            return "=";
        case kTokenPlus:
            return "+";
        case kTokenMinus:
            return "-";
        case kTokenCompareEquals:
            return "==";
        case kTokenOpenParen:
            return "(";
        case kTokenCloseParen:
            return ")";
        case kTokenOpenBrace:
            return "{";
        case kTokenCloseBrace:
            return "}";
        case kTokenSemiColon:
            return ";";
        case kTokenLoop:
            return "loop";
        case kTokenBreak:
            return "break";
        case kTokenEOF:
            return "EOF";
        case kTokenCount:
		default:
            return "invalid";
    }

    return "invalid";
}
