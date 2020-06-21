#include <ast.h>
#include <executioncontext.h>
#include <lexer.h>
#include <ast/node.h>
#include <ast/visitor/astdisplayer.h>
#include <ast/visitor/astdebugexecutor.h>

#include <iostream>

static const char* K_TEST_PROGRAM
{
"void main()\n"
"{\n"
"    s32 a = ReadS32();\n"
"    if (a)\n"
"    {\n"
"        Print(0);\n"
"    }\n"
"    else\n"
"    {\n"
"        Print(1);\n"
"    }\n"
"}\n"
};

int main()
{
	Lexer lexer{ K_TEST_PROGRAM };

	ExecutionContext ec;
	AST::Node* n{ AST::BuildAST(lexer, ec) };

	AST::ASTDisplayer displayer;
	n->Accept(displayer);

	AST::ASTDebugExecutor executor;
	n->Accept(executor);

	delete n;
	return 0;
}
