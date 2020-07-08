#include <ast/astbuilder.h>
#include <ast/compilationcontext.h>
#include <ast/lexer.h>
#include <ast/node/functiondeclarationnode.h>
#include <ast/node/node.h>
#include <ast/node/visitor/astdebugexecutor.h>
#include <virtualmachine/vmcontext.h>

#include <iostream>

static const char* K_TEST_PROGRAM
{
"void TestFunction(s32 arg)\n"
"{\n"
"    if (arg)\n"
"    {\n"
"        Print(arg);\n"
"        TestFunction(arg - 1);\n"
"    }\n"
"}\n"
"\n"
"void main()\n"
"{\n"
"    s32 a = 1;\n"
"    while (a)\n"
"    {\n"
"        s32 a = ReadS32();\n"
"        TestFunction(a);"
"    }\n"
"    Print(a);\n"
"}\n"
};

int main()
{
	Lexer lexer{ K_TEST_PROGRAM };

	CompilationContext context;
	AST::Node* rootNode{ AST::BuildAST(lexer, context) };

	if (AST::FunctionDeclarationNode* mainFunction = context.FindFunction("main"))
	{
		AST::ASTDebugExecutor executor{ context };
		rootNode->Accept(executor);
		mainFunction->GetFunctionBody()->Accept(executor);

		//VirtualMachine::VMContext vmContext{};
		//vmContext.LoadBinaryCode(binaryCode);
		//vmContext.RunInstructions();
	}
	else
	{
		std::cerr << "Could not find 'main' function.\n";
	}

	delete rootNode;
	return 0;
}
