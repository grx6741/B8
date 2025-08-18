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
	case kTokenNumber:	  LOG_INFO("TokenNumber[%d, %d] :: %s\n", token.row, token.col, token.value); break;
	case kTokenAssign:	  LOG_INFO("TokenAssign[%d, %d]\n", token.row, token.col); break;
	case kTokenPlus:	  LOG_INFO("TokenPlus[%d, %d]\n", token.row, token.col); break;
	case kTokenCompareEquals: LOG_INFO("TokenCompareEquals[%d, %d]\n", token.row, token.col); break;
	case kTokenOpenParen:	  LOG_INFO("TokenOpenParen[%d, %d]\n", token.row, token.col); break;
	case kTokenCloseParen:	  LOG_INFO("TokenCloseParen[%d, %d]\n", token.row, token.col); break;
	case kTokenOpenBrace:	  LOG_INFO("TokenOpenBrace[%d, %d]\n", token.row, token.col); break;
	case kTokenCloseBrace:	  LOG_INFO("TokenCloseBrace[%d, %d]\n", token.row, token.col); break;
	case kTokenSemiColon:	  LOG_INFO("TokenSemiColon[%d, %d]\n", token.row, token.col); break;
	case kTokenEOF:		  LOG_INFO("TokenEOF[%d, %d]\n", token.row, token.col); break;
	case kTokenCount:	  LOG_INFO("Invalid Token[%d, %d]\n", token.row, token.col); break;
    }
}
