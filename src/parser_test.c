#define BUFFER(str) fmemopen((void*)(str), strlen((str)), "r")

ast_node_t* ast_init(const char* input) {
    lexer_t L = Lexer(BUFFER(input), "temp");
    parser_t P = Parser(&L);
    return ParseProgram(&P);
}

TEST(ParserAssignment) {
	ast_node_t* AST = ast_init("int a;");
	FAIL_IF_NOT(AST != NULL, "AST should not be NULL");
	FAIL_IF_NOT(AST->type == kASTnodeDeclare, "Expected %s but got %s",
			ASTnodeTypeToString(kASTnodeDeclare), 
			ASTnodeTypeToString(AST->type));
	FAIL_IF_NOT(strcmp(AST->declaration.name, "a") == 0, 
			"Expected variable name 'a' but got '%s'", AST->declaration.name);
	ASTfree(AST);
	return TEST_PASSED();
}

TEST(ParserAssignmentConstant) {
    ast_node_t* AST = ast_init("a = 42;");
    FAIL_IF_NOT(AST != NULL, "AST should not be NULL");
    FAIL_IF_NOT(AST->type == kASTnodeAssign, "Expected assignment node");
    FAIL_IF_NOT(strcmp(AST->assignment.lhs, "a") == 0, 
        "Expected LHS 'a' but got '%s'", AST->assignment.lhs);
    FAIL_IF_NOT(AST->assignment.rhs != NULL, "RHS should not be NULL");
    FAIL_IF_NOT(AST->assignment.rhs->type == kASTnodeConstant, 
        "Expected RHS to be constant");
    FAIL_IF_NOT(AST->assignment.rhs->value.constant == 42, 
        "Expected constant 42 but got %d", AST->assignment.rhs->value.constant);
    ASTfree(AST);
    return TEST_PASSED();
}

TEST(ParserAssignmentIdentifier) {
    ast_node_t* AST = ast_init("a = b;");
    FAIL_IF_NOT(AST != NULL, "AST should not be NULL");
    FAIL_IF_NOT(AST->type == kASTnodeAssign, "Expected assignment node");
    FAIL_IF_NOT(strcmp(AST->assignment.lhs, "a") == 0, "Expected LHS 'a'");
    FAIL_IF_NOT(AST->assignment.rhs->type == kASTnodeIdentifier, 
        "Expected RHS to be identifier");
    FAIL_IF_NOT(strcmp(AST->assignment.rhs->value.name, "b") == 0, 
        "Expected identifier 'b'");
    ASTfree(AST);
    return TEST_PASSED();
}

TEST(ParserBinaryAdd) {
    ast_node_t* AST = ast_init("a = b + c;");
    FAIL_IF_NOT(AST != NULL, "AST should not be NULL");
    FAIL_IF_NOT(AST->type == kASTnodeAssign, "Expected assignment");
    FAIL_IF_NOT(AST->assignment.rhs->type == kASTnodeBinaryOp, 
        "Expected binary operation");
    
    ast_node_t* binop = AST->assignment.rhs;
    FAIL_IF_NOT(binop->binary_op.op == kBinaryAdd, "Expected addition operation");
    FAIL_IF_NOT(binop->binary_op.lhs->type == kASTnodeIdentifier, "Expected LHS identifier");
    FAIL_IF_NOT(binop->binary_op.rhs->type == kASTnodeIdentifier, "Expected RHS identifier");
    FAIL_IF_NOT(strcmp(binop->binary_op.lhs->value.name, "b") == 0, "Expected LHS 'b'");
    FAIL_IF_NOT(strcmp(binop->binary_op.rhs->value.name, "c") == 0, "Expected RHS 'c'");
    
    ASTfree(AST);
    return TEST_PASSED();
}

TEST(ParserIf) {
    ast_node_t* AST = ast_init("if (a < b) { a = 1; } else { a = 2; }");
    FAIL_IF_NOT(AST != NULL, "AST should not be NULL");
    FAIL_IF_NOT(AST->type == kASTnodeIf, "Expected if statement");
    FAIL_IF_NOT(AST->if_node.condition != NULL, "Condition should not be NULL");
    FAIL_IF_NOT(AST->if_node.condition->type == kASTnodeBinaryOp, 
        "Expected condition to be binary operation");
    FAIL_IF_NOT(AST->if_node.if_block != NULL, "If block should not be NULL");
    FAIL_IF_NOT(AST->if_node.else_block != NULL, "Else block should not be NULL");
    
    ASTfree(AST);
    return TEST_PASSED();
}

TEST(ParserLoop) {
    ast_node_t* AST = ast_init("loop { a = a + 1; }");
    FAIL_IF_NOT(AST != NULL, "AST should not be NULL");
    FAIL_IF_NOT(AST->type == kASTnodeLoop, "Expected loop statement");
    FAIL_IF_NOT(AST->loop.block != NULL, "Loop block should not be NULL");
    
    ASTfree(AST);
    return TEST_PASSED();
}

TEST(ParserFunctionCall) {
    ast_node_t* AST = ast_init("out(a);");
    FAIL_IF_NOT(AST != NULL, "AST should not be NULL");
    FAIL_IF_NOT(AST->type == kASTnodeFuncCall, "Expected function call");
    FAIL_IF_NOT(strcmp(AST->func_call.name, "out") == 0, 
        "Expected function name 'out'");
    FAIL_IF_NOT(AST->func_call.arg != NULL, "Expected argument");
    FAIL_IF_NOT(AST->func_call.arg->type == kASTnodeIdentifier, 
        "Expected argument to be identifier");
    FAIL_IF_NOT(strcmp(AST->func_call.arg->value.name, "a") == 0, 
        "Expected argument 'a'");
    
    ASTfree(AST);
    return TEST_PASSED();
}
