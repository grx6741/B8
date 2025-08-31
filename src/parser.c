#include "parser.h"

#define LOG_ENABLED 0
#define LOG_PREFIX "PARSER"
#include "util.h"

#include "assert.h"
#include "string.h"

#define CONSUME( P, type ) __consume( P, type, __func__ )

token_t __peek(parser_t* P);
token_t __consume(parser_t* P, TokenType type, const char* caller);

ast_node_t* __parse_stmt_list(parser_t* P);
ast_node_t* __parse_stmt(parser_t* P);
ast_node_t* __parse_block(parser_t* P);
ast_node_t* __parse_decl(parser_t* P);
ast_node_t* __parse_assign(parser_t* P);
ast_node_t* __parse_expr(parser_t* P);
ast_node_t* __parse_expr_(parser_t* P, ast_node_t* lhs);
ast_node_t* __parse_value(parser_t* P);
ast_node_t* __parse_if(parser_t* P);
ast_node_t* __parse_loop(parser_t* P);
ast_node_t* __parse_break(parser_t* P);

parser_t Parser( lexer_t* lexer )
{
    assert( lexer );
    parser_t P;
    P.L = lexer;
    P.curr = LexerNextToken( P.L );
    return P;
}

// program	    := stmt_list
ast_node_t* ParseProgram( parser_t* P )
{
    LOG_INFO( "Parsing Program\n" );
    return __parse_stmt_list( P );
}

// stmt_list	:= stmt*
// stmt := decl | assign | if | block |  func_call | loop | break
ast_node_t* __parse_stmt_list( parser_t* P )
{
    LOG_INFO( "Parsing StmtList\n" );
    ast_node_t* node = NULL;
    ast_node_t* next = NULL;

    while ( __peek( P ).type == kTokenInt			|| // Declaration
            __peek( P ).type == kTokenIdentifier	|| // Assignment
            __peek( P ).type == kTokenLoop		|| // Loop
            __peek( P ).type == kTokenBreak		|| // Break
            __peek( P ).type == kTokenIf			|| // If
            __peek( P ).type == kTokenOpenBrace      // Block
    ) {
        ast_node_t* stmt = __parse_stmt( P );
        if ( !node ) {
            node = stmt;
            next = stmt;
        }
        else {
            next->next = stmt;
            next = stmt;
        }
    }

    return node;
}

// stmt		:= decl | assign | if | block
ast_node_t* __parse_stmt( parser_t* P )
{
    LOG_INFO( "Parsing Stmt\n" );
    if ( __peek( P ).type == kTokenInt )
        return __parse_decl( P );

    if ( __peek( P ).type == kTokenIdentifier )
        return __parse_assign( P );

    if ( __peek( P ).type == kTokenIf )
        return __parse_if( P );

    if ( __peek( P ).type == kTokenOpenBrace )
        return __parse_block( P );

    if ( __peek( P ).type == kTokenLoop )
        return __parse_loop( P );

    if ( __peek( P ).type == kTokenBreak )
        return __parse_break( P );

    LOG_ERROR( "Invalid Statement."
               "Expected \"int\", \"variable\", \"if\", \"{\"\n" );

    return NULL;
}

ast_node_t* __parse_loop(parser_t* P)
{
    LOG_INFO( "Parsing Loop\n" );
	CONSUME(P, kTokenLoop);

	ast_node_t* loop = ASTnode(kASTnodeLoop);
	loop->loop.block = __parse_block(P);

	return loop;
}

ast_node_t* __parse_break(parser_t* P)
{
    LOG_INFO( "Parsing Break\n" );
	CONSUME(P, kTokenBreak);
	CONSUME(P, kTokenSemiColon);

	return ASTnode(kASTnodeBreak);
}

// decl	    := 'int' identifier ';'
ast_node_t* __parse_decl( parser_t* P )
{
    LOG_INFO( "Parsing Decl\n" );
    CONSUME( P, kTokenInt );
    token_t tok = CONSUME( P, kTokenIdentifier );
    CONSUME( P, kTokenSemiColon );

    ast_node_t* node = ASTnode( kASTnodeDeclare );
    strncpy( node->declaration.name, tok.name, MAX_IDENTIFIER_NAME_LEN );

    return node;
}

ast_node_t* ParseFuncCall( parser_t* P, token_t func_name_token )
{
    ast_node_t* node = ASTnode( kASTnodeFuncCall );
    strncpy( node->func_call.name, func_name_token.name, MAX_IDENTIFIER_NAME_LEN );

	CONSUME( P, kTokenOpenParen );

	token_t arg_tok = CONSUME(P, kTokenIdentifier );

	CONSUME( P, kTokenCloseParen );

	CONSUME( P, kTokenSemiColon );

	ast_node_t* arg = ASTnode(kASTnodeIdentifier);
    strncpy( arg->value.name, arg_tok.name, MAX_IDENTIFIER_NAME_LEN );

	node->func_call.arg = arg;

	return node;
}

// assign := identifier '=' expr ';'
ast_node_t* __parse_assign( parser_t* P )
{
    LOG_INFO( "Parsing Assigment\n" );
    ast_node_t* node = ASTnode( kASTnodeAssign );

    token_t iden = CONSUME( P, kTokenIdentifier );

	// func_call   := identifier '(' identifier ')' ';'
	if ( __peek( P ).type == kTokenOpenParen ) {
		return ParseFuncCall( P, iden );
	}

    strncpy( node->assignment.lhs, iden.name, MAX_IDENTIFIER_NAME_LEN );

    CONSUME( P, kTokenAssign );

    node->assignment.rhs = __parse_expr( P );

    CONSUME( P, kTokenSemiColon );

    return node;
}

// expr := value expr_
ast_node_t* __parse_expr( parser_t* P )
{
    LOG_INFO( "Parsing Expr\n" );
    ast_node_t* lhs = __parse_value( P );
    return __parse_expr_( P, lhs );
}

// expr_ := (op value) | <empty>
ast_node_t* __parse_expr_( parser_t* P, ast_node_t* lhs )
{
    LOG_INFO( "Parsing Expr_\n" );
    ast_node_t* node = ASTnode( kASTnodeBinaryOp );
    static TokenType token_to_op_table[kBinaryTypeCount] = {
        [kBinaryAdd] = kTokenPlus,
        [kBinarySub] = kTokenMinus,
        [kBinaryLT] = kTokenCompareLessThan,
        [kBinaryLE] = kTokenCompareLessThanEquals,
        [kBinaryGT] = kTokenCompareGreaterThan,
        [kBinaryGE] = kTokenCompareGreaterThanEquals,
        [kBinaryEQ] = kTokenCompareEquals,
        [kBinaryNE] = kTokenCompareNotEquals,
    };

    node->binary_op.lhs = lhs;
    bool valid_op = false;
    for ( int i = 0; i < kBinaryTypeCount; i++ ) {
        if ( __peek( P ).type == token_to_op_table[i] ) {
            node->binary_op.op = i;
            CONSUME( P, token_to_op_table[i] );
            valid_op = true;
        }
    }

    if ( !valid_op ) {
        return lhs;
    }

    node->binary_op.rhs = __parse_value( P );
    return node;
}

// value := identifier | constant
ast_node_t* __parse_value( parser_t* P )
{
    LOG_INFO( "Parsing Value\n" );
    if ( __peek( P ).type == kTokenIdentifier ) {
        token_t iden = CONSUME( P, kTokenIdentifier );
        ast_node_t* node = ASTnode( kASTnodeIdentifier );

        strncpy( node->value.name, iden.name, MAX_IDENTIFIER_NAME_LEN );

        return node;
    }

    if ( __peek( P ).type == kTokenNumber ) {
        token_t number = CONSUME( P, kTokenNumber );
        ast_node_t* node = ASTnode( kASTnodeConstant );

        node->value.constant = number.value;
        return node;
    }

    LOG_ERROR( "Expected Constant or Identifier at %d,%d\n", P->L->row, P->L->col );

    return NULL;
}

ast_node_t* __parse_if( parser_t* P )
{
    LOG_INFO( "Parsing If\n" );

    CONSUME( P, kTokenIf );

    CONSUME( P, kTokenOpenParen );

    ast_node_t* condition = __parse_expr( P );

    CONSUME( P, kTokenCloseParen );

    ast_node_t* if_block = __parse_block( P );

    CONSUME( P, kTokenElse );
    LOG_INFO( "Parsing Else\n" );

    ast_node_t* else_block = __parse_block( P );

    ast_node_t* if_node = ASTnode( kASTnodeIf );
    if_node->if_node.condition = condition;
    if_node->if_node.if_block = if_block;
    if_node->if_node.else_block = else_block;

    return if_node;
}

ast_node_t* __parse_block( parser_t* P )
{
    LOG_INFO( "Parsing Block\n" );

    CONSUME( P, kTokenOpenBrace );

    ast_node_t* stmt_list = __parse_stmt_list( P );

    CONSUME( P, kTokenCloseBrace );

    return stmt_list;
}

token_t __peek( parser_t* P )
{
    return P->curr;
}

token_t __consume( parser_t* P, TokenType type, const char* caller )
{
    assert( P->L );
    if ( __peek( P ).type != type ) {
        LOG_ERROR( "[%s] Expected Token Type %s, but got %s at %d,%d\n",
                   caller,
                   TokenTypeToStr( type ),
                   TokenTypeToStr( P->curr.type ),
                   P->L->row,
                   P->L->col );
		fprintf(stderr, "%s:%d:%d Expected \"%s\" but got \"%s\"\n", 
				P->L->file_name, P->L->row, P->L->col, 
				TokenTypeToStr( type ),
				TokenTypeToStr( P->curr.type )
			   );
    }

    token_t token = __peek( P );
    P->curr = LexerNextToken( P->L );

    LOG_INFO( "Consumed %s\n", TokenTypeToStr( type ) );

    return token;
}
