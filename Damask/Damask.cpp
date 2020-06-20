﻿#include <ast.h>
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
"    s32 b = ReadS32();\n"
"\n"
"    Print(a + b * 5);\n"
"    Print(a / b);\n"
"\n"
"    s32 c = (a + b) * a;\n"
"    Print(c);\n"
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