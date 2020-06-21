#include <astbuilder.h>
#include <compilationcontext.h>
#include <lexer.h>
#include <ast/node.h>
#include <ast/visitor/astdisplayer.h>
#include <ast/visitor/astdebugexecutor.h>

#include <iostream>

static const char* K_TEST_PROGRAM
{
"void main()\n"
"{\n"
"    s32 a = 1;\n"
"    while (a)\n"
"    {\n"
"        s32 a = ReadS32();\n"
"    }\n"
"    Print(a);\n"
"}\n"
};

int main()
{
	Lexer lexer{ K_TEST_PROGRAM };

	CompilationContext context;
	AST::Node* n{ AST::BuildAST(lexer, context) };

	AST::ASTDisplayer displayer;
	n->Accept(displayer);

	AST::ASTDebugExecutor executor;
	n->Accept(executor);

	delete n;
	return 0;
}
