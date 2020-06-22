#pragma once

class CompilationContext;
struct Lexer;

namespace AST
{
	class Node;

	Node* BuildAST(Lexer& lexer, CompilationContext& compilationContext);
}