#include "token.h"

#define LOG_ENABLED 1
#define LOG_PREFIX "TOKEN"
#include "util.h"

void LogToken(token_t token)
{
    switch (token.type) {
	case kTokenInt:		  LOG_INFO("TokenInt[%d, %d]\n", token.row, token.col); break;
	case kTokenIf:	    	  LOG_INFO("TokenIf[%d, %d]\n", token.row, token.col); break;
	case kTokenIdentifier:	  LOG_INFO("TokenIdentifier[%d, %d] :: %s\n", token.row, token.col, token.name); break;
	case kTokenNumber:	  LOG_INFO("TokenNumber[%d, %d] :: %d\n", token.row, token.col, (int)token.value); break;
	case kTokenAssign:	  LOG_INFO("TokenAssign[%d, %d]\n", token.row, token.col); break;
	case kTokenPlus:	  LOG_INFO("TokenPlus[%d, %d]\n", token.row, token.col); break;
	case kTokenMinus:	  LOG_INFO("TokenMinus[%d, %d]\n", token.row, token.col); break;
	case kTokenCompareEquals: LOG_INFO("TokenCompareEquals[%d, %d]\n", token.row, token.col); break;
	case kTokenOpenParen:	  LOG_INFO("TokenOpenParen[%d, %d]\n", token.row, token.col); break;
	case kTokenCloseParen:	  LOG_INFO("TokenCloseParen[%d, %d]\n", token.row, token.col); break;
	case kTokenOpenBrace:	  LOG_INFO("TokenOpenBrace[%d, %d]\n", token.row, token.col); break;
	case kTokenCloseBrace:	  LOG_INFO("TokenCloseBrace[%d, %d]\n", token.row, token.col); break;
	case kTokenSemiColon:	  LOG_INFO("TokenSemiColon[%d, %d]\n", token.row, token.col); break;
	case kTokenEOF:		  LOG_INFO("TokenEOF[%d, %d]\n", token.row, token.col); break;
	case kTokenCount:	  LOG_INFO("Invalid Token[%d, %d] %c\n", token.row, token.col, token.name[0]); break;
    }
}

const char* TokenTypeToStr(TokenType type)
{
    switch (type) {
	case kTokenInt: return "kTokenInt";
	case kTokenIf: return "kTokenIf";
	case kTokenIdentifier: return "kTokenIdentifier";
	case kTokenNumber: return "kTokenNumber";
	case kTokenAssign: return "kTokenAssign"; 
	case kTokenPlus: return "kTokenPlus"; 
	case kTokenMinus: return "kTokenMinus"; 
	case kTokenCompareEquals: return "kTokenCompareEquals";
	case kTokenOpenParen: return "kTokenOpenParen"; 
	case kTokenCloseParen: return "kTokenCloseParen";
	case kTokenOpenBrace: return "kTokenOpenBrace"; 
	case kTokenCloseBrace: return "kTokenCloseBrace";
	case kTokenSemiColon: return "kTokenSemiColon";
	case kTokenEOF: return "kTokenEOF";
	case kTokenCount: return "kTokenCount";
    }
}
