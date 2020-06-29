#include <compiler/astbuilder.h>
#include <compiler/compilationcontext.h>
#include <compiler/lexer.h>
#include <compiler/ast/functiondeclarationnode.h>
#include <compiler/ast/node.h>
#include <virtualmachine/vmbinarycompiler.h>
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
		VirtualMachine::VMBinaryCompiler binaryCompiler{};
		rootNode->Accept(binaryCompiler);

		//TODO: accept all functions & handle function locating in binary

		mainFunction->Accept(binaryCompiler);
		std::vector<char> binaryCode{ binaryCompiler.BuildBinaryCode() };

		VirtualMachine::VMContext vmContext{};
		vmContext.LoadBinaryCode(binaryCode);
		vmContext.RunInstructions();
	}
	else
	{
		std::cerr << "Could not find 'main' function.\n";
	}

	delete rootNode;
	return 0;
}
