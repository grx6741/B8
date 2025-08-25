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
            return "kTokenElse";
        case kTokenCompareNotEquals:
            return "kTokenCompareNotEquals";
        case kTokenCompareLessThan:
            return "kTokenCompareLessThan";
        case kTokenCompareLessThanEquals:
            return "kTokenCompareLessThanEquals";
        case kTokenCompareGreaterThan:
            return "kTokenCompareGreaterThan";
        case kTokenCompareGreaterThanEquals:
            return "kTokenCompareGreaterThanEquals";
        case kTokenInt:
            return "kTokenInt";
        case kTokenIf:
            return "kTokenIf";
        case kTokenIdentifier:
            return "kTokenIdentifier";
        case kTokenNumber:
            return "kTokenNumber";
        case kTokenAssign:
            return "kTokenAssign";
        case kTokenPlus:
            return "kTokenPlus";
        case kTokenMinus:
            return "kTokenMinus";
        case kTokenCompareEquals:
            return "kTokenCompareEquals";
        case kTokenOpenParen:
            return "kTokenOpenParen";
        case kTokenCloseParen:
            return "kTokenCloseParen";
        case kTokenOpenBrace:
            return "kTokenOpenBrace";
        case kTokenCloseBrace:
            return "kTokenCloseBrace";
        case kTokenSemiColon:
            return "kTokenSemiColon";
        case kTokenEOF:
            return "kTokenEOF";
        case kTokenCount:
            return "kTokenCount";
    }

    return NULL;
}
